#include "androidhooks.h"
#include <jni.h>
#include <android/log.h>

static JavaVM *jvm = NULL;
static jclass gameActivityClass;
static jmethodID midShowSettings;

jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved)
{
    JNIEnv *env = NULL;
    jvm = vm;

    if ((*jvm).GetEnv((void **)&env, JNI_VERSION_1_4) != JNI_OK) {
        __android_log_print(ANDROID_LOG_ERROR, "CorsixTH", "Failed to get JNI Env");
        return JNI_VERSION_1_4;
    }

    gameActivityClass = (jclass)env->NewGlobalRef((jclass)(env->FindClass("uk/co/armedpineapple/cth/GameActivity")));
    midShowSettings = env->GetStaticMethodID(gameActivityClass, "showSettings", "()V");

//    register_methods(env, "org/libsdl/app/SDLActivity", SDLActivity_tab, SDL_arraysize(SDLActivity_tab));
//    register_methods(env, "org/libsdl/app/SDLInputConnection", SDLInputConnection_tab, SDL_arraysize(SDLInputConnection_tab));
//    register_methods(env, "org/libsdl/app/SDLAudioManager", SDLAudioManager_tab, SDL_arraysize(SDLAudioManager_tab));
//    register_methods(env, "org/libsdl/app/SDLControllerManager", SDLControllerManager_tab, SDL_arraysize(SDLControllerManager_tab));

    return JNI_VERSION_1_4;
}

static int showSettings(lua_State *L) {
    JNIEnv *env;
    jvm->AttachCurrentThread(&env, NULL);
    env->CallStaticVoidMethod(gameActivityClass, midShowSettings);
    return 0;
}

void registerAndroidLuaFunctions(const lua_register_state *pState)
{
    add_lua_function(pState, showSettings, "showSettings");
}