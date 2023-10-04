#include "androidhooks.h"
#include "androidevents.h"
#include <jni.h>
#include <android/log.h>
#include <SDL_events.h>
#include <iostream>

static JavaVM *jvm = nullptr;

static jclass gameActivityClass, gameConfigClass, eventsClass;
static jobject gameActivitySingleton, gameEventHandler;

static jmethodID midShowSettings, midShowLoad, midShowSave, midUpdateSaveGameDatabase,
        midShowAchievements, midSignIn;
static jmethodID midConfigGetAdvisorEnabled, midConfigGetAudioEnabled, midConfigGetSfxEnabled,
        midConfigGetMusicEnabled, midConfigGetLanguage, midConfigGetAnnouncerEnabled,
        midConfigGetAnnouncerVolume, midConfigGetSfxVolume, midConfigGetMusicVolume,
        midConfigGetScrollMode, midConfigGetEdgeScroll;
static jmethodID midEventOnCure, midEventOnKill, midEventOnCampaignLevelComplete,
        midEventOnBankBalanceChanged, midEventOnLoanTaken;
static jmethodID midReportError;

static void ensureEnv(JNIEnv **env) {
    int result = jvm->GetEnv((void **) env, JNI_VERSION_1_4);
    if (result == JNI_EDETACHED) {
        if (jvm->AttachCurrentThread(env, nullptr) != 0) {
            std::cerr << "Failed to attach" << std::endl;
        }
    } else if (result == JNI_EVERSION) {
        std::cerr << "Version not supported" << std::endl;
    }
}

jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    jvm = vm;

    JNIEnv *env;
    ensureEnv(&env);

    // Init global references so that we don't need to keep getting them.

    // Classes
    gameActivityClass = (jclass) env->NewGlobalRef(
            (jclass) (env->FindClass("uk/co/armedpineapple/cth/GameActivity")));
    gameConfigClass = (jclass) env->NewGlobalRef(
            (jclass) (env->FindClass("uk/co/armedpineapple/cth/GameConfiguration")));
    eventsClass = (jclass) env->NewGlobalRef(
            (jclass) (env->FindClass("uk/co/armedpineapple/cth/GameEventHandler")));

    // Singletons
    jfieldID singletonFieldId = env->GetStaticFieldID(gameActivityClass, "singleton",
                                                      "Luk/co/armedpineapple/cth/GameActivity;");
    jobject singleton = env->GetStaticObjectField(gameActivityClass, singletonFieldId);
    gameActivitySingleton = (jobject) env->NewGlobalRef(singleton);

    // Activity Methods
    midShowSettings = env->GetStaticMethodID(gameActivityClass, "showSettings", "()V");
    midShowLoad = env->GetStaticMethodID(gameActivityClass, "showLoad", "()V");
    midShowSave = env->GetStaticMethodID(gameActivityClass, "showSave", "()V");
    midUpdateSaveGameDatabase = env->GetStaticMethodID(gameActivityClass, "onSaveGameChanged",
                                                       "([BIJ[B[B)V");
    midReportError = env->GetStaticMethodID(gameActivityClass, "onGameError", "([B[B)V");
    midShowAchievements = env->GetStaticMethodID(gameActivityClass, "showAchievements", "()V");
    midSignIn = env->GetStaticMethodID(gameActivityClass, "signIn", "()V");

    // Configuration Methods
    midConfigGetAdvisorEnabled = env->GetMethodID(gameConfigClass, "getAdvisorEnabled", "()Z");
    midConfigGetAudioEnabled = env->GetMethodID(gameConfigClass, "getAudioEnabled", "()Z");
    midConfigGetSfxEnabled = env->GetMethodID(gameConfigClass, "getSfxEnabled", "()Z");
    midConfigGetMusicEnabled = env->GetMethodID(gameConfigClass, "getMusicEnabled", "()Z");
    midConfigGetAnnouncerEnabled = env->GetMethodID(gameConfigClass, "getAnnouncerEnabled", "()Z");
    midConfigGetEdgeScroll = env->GetMethodID(gameConfigClass, "getEdgeScroll", "()Z");
    midConfigGetAnnouncerVolume = env->GetMethodID(gameConfigClass, "getAnnouncerVolume", "()I");
    midConfigGetSfxVolume = env->GetMethodID(gameConfigClass, "getSfxVolume", "()I");
    midConfigGetMusicVolume = env->GetMethodID(gameConfigClass, "getMusicVolume", "()I");
    midConfigGetLanguage = env->GetMethodID(gameConfigClass, "getLanguage", "()Ljava/lang/String;");
    midConfigGetScrollMode = env->GetMethodID(gameConfigClass, "getScrollMode", "()I");

    // Events Methods
    midEventOnCure = env->GetMethodID(eventsClass, "onCure", "()V");
    midEventOnKill = env->GetMethodID(eventsClass, "onKill", "()V");
    midEventOnCampaignLevelComplete = env->GetMethodID(eventsClass, "onCampaignLevelComplete",
                                                       "(I)V");
    midEventOnBankBalanceChanged = env->GetMethodID(eventsClass, "onBankBalanceChanged",
                                                    "(J)V");
    midEventOnLoanTaken = env->GetMethodID(eventsClass, "onLoanTaken", "(J)V");

    // Game Event handler

    jmethodID midGetGameEventHandler = env->GetMethodID(gameActivityClass, "getGameEventHandler",
                                                        "()Luk/co/armedpineapple/cth/GameEventHandler;");
    jobject localGameEventHandler = env->CallObjectMethod(gameActivitySingleton,
                                                          midGetGameEventHandler);
    gameEventHandler = env->NewGlobalRef(localGameEventHandler);

    return JNI_VERSION_1_4;
}

static int checkStackSize(lua_State *L, int expected) {
    int stackSize = lua_gettop(L);
    if (stackSize != expected) {
        return luaL_error(L, "Unexpected stack size. Was %d, expected %d.", stackSize, expected);
    }
    return 0;
}

static int reportError(lua_State *L) {
    checkStackSize(L, 2);
    JNIEnv *env;
    ensureEnv(&env);

    const char *handler = lua_tostring(L, 1);
    const int handlerLen = static_cast<int>(luaL_len(L, 1));
    const char *stack = lua_tostring(L, 2);
    const int stackLen = static_cast<int>(luaL_len(L, 2));

    jbyteArray handlerArray = env->NewByteArray(handlerLen);
    env->SetByteArrayRegion(handlerArray, 0, handlerLen, (const jbyte *) handler);

    jbyteArray stackArray = env->NewByteArray(stackLen);
    env->SetByteArrayRegion(stackArray, 0, stackLen, (const jbyte *) stack);

    env->CallStaticVoidMethod(gameActivityClass, midReportError, handlerArray, stackArray);

    env->DeleteLocalRef(handlerArray);
    env->DeleteLocalRef(stackArray);

    return 0;
}

int reportError(const char *stack) {
    JNIEnv *env;
    ensureEnv(&env);

    jbyteArray stackArray = env->NewByteArray(static_cast<int>(strlen(stack)));
    env->SetByteArrayRegion(stackArray, 0, static_cast<int>(strlen(stack)), (const jbyte *) stack);

    env->CallStaticVoidMethod(gameActivityClass, midReportError, nullptr, stackArray);
    return 0;
}

int reportError() {
    JNIEnv *env;
    ensureEnv(&env);

    env->CallStaticVoidMethod(gameActivityClass, midReportError, nullptr, nullptr);
    return 0;
}

static int showSettings(lua_State *L) {
    JNIEnv *env;
    ensureEnv(&env);
    env->CallStaticVoidMethod(gameActivityClass, midShowSettings);
    return 0;
}

static int showLoad(lua_State *L) {
    JNIEnv *env;
    ensureEnv(&env);
    env->CallStaticVoidMethod(gameActivityClass, midShowLoad);
    return 0;
}

static int showSave(lua_State *L) {
    JNIEnv *env;
    ensureEnv(&env);
    env->CallStaticVoidMethod(gameActivityClass, midShowSave);
    return 0;
}

static int signIn(lua_State *L) {
    JNIEnv *env;
    ensureEnv(&env);
    env->CallStaticVoidMethod(gameActivityClass, midSignIn);
    return 0;
}

static int showAchievements(lua_State *L) {
    JNIEnv *env;
    ensureEnv(&env);
    env->CallStaticVoidMethod(gameActivityClass, midShowAchievements);
    return 0;
}

static int updateSaveGameDatabase(lua_State *L) {
    checkStackSize(L, 5);
    const char *saveName = lua_tostring(L, 1);
    const int saveLen = static_cast<int>(luaL_len(L, 1));
    const auto rep = static_cast<jint>(luaL_checkinteger(L, 2));
    const auto money = static_cast<jlong>(luaL_checkinteger(L, 3));
    const char *level = lua_tostring(L, 4);
    const int levelLen = static_cast<int>(luaL_len(L, 4));
    const char *ssPath = lua_tostring(L, 5);
    const int ssLen = static_cast<int>(luaL_len(L, 5));

    JNIEnv *env;
    ensureEnv(&env);

    jbyteArray saveString = env->NewByteArray(saveLen);
    env->SetByteArrayRegion(saveString, 0, saveLen, (const jbyte *) saveName);

    jbyteArray levelString = env->NewByteArray(levelLen);
    env->SetByteArrayRegion(levelString, 0, levelLen, (const jbyte *) level);

    jbyteArray ssString = env->NewByteArray(ssLen);
    env->SetByteArrayRegion(ssString, 0, ssLen, (const jbyte *) ssPath);

    env->CallStaticVoidMethod(gameActivityClass, midUpdateSaveGameDatabase, saveString, rep, money,
                              levelString, ssString);

    env->DeleteLocalRef(saveString);
    env->DeleteLocalRef(levelString);
    env->DeleteLocalRef(ssString);

    return 0;
}

static int onCureEvent(lua_State *L) {
    JNIEnv *env;
    ensureEnv(&env);
    env->CallVoidMethod(gameEventHandler, midEventOnCure);
    return 0;
}

static int onKillEvent(lua_State *L) {
    JNIEnv *env;
    ensureEnv(&env);
    env->CallVoidMethod(gameEventHandler, midEventOnKill);
    return 0;
}

static int onBankBalanceChangedEvent(lua_State *L) {
    checkStackSize(L, 1);
    JNIEnv *env;
    ensureEnv(&env);

    const auto balanceDelta = static_cast<jlong>(luaL_checkinteger(L, 1));

    env->CallVoidMethod(gameEventHandler, midEventOnBankBalanceChanged, balanceDelta);
    return 0;
}

static int onLoanTaken(lua_State *L) {
    checkStackSize(L, 1);
    JNIEnv *env;
    ensureEnv(&env);

    const auto loanAmount = static_cast<jlong>(luaL_checkinteger(L, 1));

    env->CallVoidMethod(gameEventHandler, midEventOnLoanTaken, loanAmount);
    return 0;
}

static int onCampaignLevelComplete(lua_State *L) {
    checkStackSize(L, 1);
    JNIEnv *env;
    ensureEnv(&env);

    const jint level = static_cast<jint>(luaL_checkinteger(L, 1));

    env->CallVoidMethod(gameEventHandler, midEventOnCampaignLevelComplete, level);
    return 0;
}

extern "C" JNIEXPORT void JNICALL
Java_uk_co_armedpineapple_cth_GameActivity_nativeSave(JNIEnv *env, jobject activity,
                                                      jstring save_name) {
    const char *nativeString = env->GetStringUTFChars(save_name, nullptr);
    pushEvent(SDL_USEREVENT_SAVE, (void *) nativeString);
}

extern "C" JNIEXPORT void JNICALL
Java_uk_co_armedpineapple_cth_GameActivity_nativeLoad(JNIEnv *env, jobject activity,
                                                      jstring save_name) {
    const char *nativeString = env->GetStringUTFChars(save_name, nullptr);
    pushEvent(SDL_USEREVENT_LOAD, (void *) nativeString);
}

extern "C" JNIEXPORT void JNICALL
Java_uk_co_armedpineapple_cth_GameActivity_nativeUpdateConfig(JNIEnv *env, jobject activity,
                                                              jobject gameConfig) {
    jboolean advisorEnabled = env->CallBooleanMethod(gameConfig, midConfigGetAdvisorEnabled);
    jboolean sfxEnabled = env->CallBooleanMethod(gameConfig, midConfigGetSfxEnabled);
    jboolean musicEnabled = env->CallBooleanMethod(gameConfig, midConfigGetMusicEnabled);
    jboolean announcerEnabled = env->CallBooleanMethod(gameConfig, midConfigGetAnnouncerEnabled);
    jboolean audioEnabled = env->CallBooleanMethod(gameConfig, midConfigGetAudioEnabled);
    jboolean edgeScroll = env->CallBooleanMethod(gameConfig, midConfigGetEdgeScroll);

    jint sfxVolume = env->CallIntMethod(gameConfig, midConfigGetSfxVolume);
    jint musicVolume = env->CallIntMethod(gameConfig, midConfigGetMusicVolume);
    jint announcerVolume = env->CallIntMethod(gameConfig, midConfigGetAnnouncerVolume);

    jint scrollMode = env->CallIntMethod(gameConfig, midConfigGetScrollMode);

    jobject languageObj = env->CallObjectMethod(gameConfig, midConfigGetLanguage);
    const char *language = env->GetStringUTFChars((jstring) languageObj, nullptr);

    auto *config = new Configuration();
    config->adviserEnabled = advisorEnabled;
    config->sfxLevel = (float) sfxVolume / 10.0f;
    config->musicLevel = (float) musicVolume / 10.0f;
    config->announcerLevel = (float) announcerVolume / 10.0f;
    config->playSfx = sfxEnabled;
    config->playAnnouncements = announcerEnabled;
    config->playMusic = musicEnabled;
    config->playAudio = audioEnabled;
    config->language = std::string(language);
    config->scrollMode = scrollMode;
    config->preventEdgeScroll = !edgeScroll;

    pushEvent(SDL_USEREVENT_CONFIGURATION, (void *) config);

    env->ReleaseStringUTFChars((jstring) languageObj, language);
}

void registerAndroidLuaFunctions(const lua_register_state *pState) {
    lua_settop(pState->L, pState->top);
    /* Make metatable the environment for registered functions */
    auto metatable = pState->metatables[static_cast<size_t>(lua_metatable::android)];
    lua_pushvalue(pState->L, metatable);

    add_lua_function(pState, showSettings, "showSettings");
    add_lua_function(pState, showLoad, "showLoad");
    add_lua_function(pState, showSave, "showSave");
    add_lua_function(pState, updateSaveGameDatabase, "updateSaveGameDatabase");
    add_lua_function(pState, reportError, "reportError");
    add_lua_function(pState, signIn, "signIn");
    add_lua_function(pState, showAchievements, "showAchievements");

    lua_setfield(pState->L, pState->main_table, "android");
}

void registerAndroidEventsLuaFunctions(const lua_register_state *pState) {
    lua_settop(pState->L, pState->top);
    /* Make metatable the environment for registered functions */
    auto metatable = pState->metatables[static_cast<size_t>(lua_metatable::android_events)];
    lua_pushvalue(pState->L, metatable);

    add_lua_function(pState, onCureEvent, "onCure");
    add_lua_function(pState, onKillEvent, "onKill");
    add_lua_function(pState, onBankBalanceChangedEvent, "onBankBalanceChanged");
    add_lua_function(pState, onLoanTaken, "onLoanTaken");
    add_lua_function(pState, onCampaignLevelComplete, "onCampaignLevelComplete");

    lua_setfield(pState->L, pState->main_table, "android_events");
}
