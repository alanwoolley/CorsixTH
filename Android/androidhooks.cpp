#include "androidhooks.h"
#include "androidevents.h"
#include <jni.h>
#include <android/log.h>
#include <SDL_events.h>

static JavaVM *jvm = NULL;
static jclass gameActivityClass;

static jmethodID midShowSettings;
static jmethodID midShowLoad;
static jmethodID midShowSave;
static jmethodID midUpdateSaveGameDatabase;

jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env = NULL;
    jvm = vm;

    if ((*jvm).GetEnv((void **) &env, JNI_VERSION_1_4) != JNI_OK) {
        __android_log_print(ANDROID_LOG_ERROR, "CorsixTH", "Failed to get JNI Env");
        return JNI_VERSION_1_4;
    }

    gameActivityClass = (jclass) env->NewGlobalRef(
            (jclass) (env->FindClass("uk/co/armedpineapple/cth/GameActivity")));
    midShowSettings = env->GetStaticMethodID(gameActivityClass, "showSettings", "()V");
    midShowLoad = env->GetStaticMethodID(gameActivityClass, "showLoad", "()V");
    midShowSave = env->GetStaticMethodID(gameActivityClass, "showSave", "()V");
    midUpdateSaveGameDatabase = env->GetStaticMethodID(gameActivityClass, "onSaveGameChanged", "([BIJ[B[B)V");

    return JNI_VERSION_1_4;
}

static int showSettings(lua_State *L) {
    JNIEnv *env;
    jvm->AttachCurrentThread(&env, NULL);
    env->CallStaticVoidMethod(gameActivityClass, midShowSettings);
    return 0;
}

static int showLoad(lua_State *L) {
    JNIEnv *env;
    jvm->AttachCurrentThread(&env, NULL);
    env->CallStaticVoidMethod(gameActivityClass, midShowLoad);
    return 0;
}

static int showSave(lua_State *L) {
    JNIEnv *env;
    jvm->AttachCurrentThread(&env, NULL);
    env->CallStaticVoidMethod(gameActivityClass, midShowSave);
    return 0;
}

static int updateSaveGameDatabase(lua_State *L) {

    int arg = lua_gettop(L);
    const char* saveName = lua_tostring(L, 1);
    const int rep = lua_tointeger(L, 2);
    const long money = lua_tointeger(L, 3);
    const char* level = lua_tostring(L, 4);
    const char* ssPath = lua_tostring(L, 5);

    JNIEnv *env;

    auto l = strlen(saveName);

    jvm->AttachCurrentThread(&env, NULL);

    jbyteArray saveString = env->NewByteArray(strlen(saveName));
    env->SetByteArrayRegion(saveString,0,strlen(saveName), (const jbyte*) saveName);

    jbyteArray levelString = env->NewByteArray(strlen(level));
    env->SetByteArrayRegion(levelString,0,strlen(level), (const jbyte*) level);

    jbyteArray ssString = env->NewByteArray(strlen(ssPath));
    env->SetByteArrayRegion(ssString,0,strlen(ssPath), (const jbyte*) ssPath);

    env->CallStaticVoidMethod(gameActivityClass, midUpdateSaveGameDatabase, saveString, rep, money, levelString, ssString);
    return 0;
}


void registerAndroidLuaFunctions(const lua_register_state *pState) {
    add_lua_function(pState, showSettings, "showSettings");
    add_lua_function(pState, showLoad, "showLoad");
    add_lua_function(pState, showSave, "showSave");
    add_lua_function(pState, updateSaveGameDatabase, "updateSaveGameDatabase");
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