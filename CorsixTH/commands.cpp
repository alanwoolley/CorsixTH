#include "commands.h"
#include "logging.h"


int sendCommandInt(JavaVM* jvm, jint cmd, jint data) {
	LOG_INFO("Sending command int data");
	JNIEnv* jEnv;
	jvm->AttachCurrentThread(&jEnv, NULL);
	jclass cls = jEnv->FindClass("uk/co/armedpineapple/cth/SDLActivity");
	jmethodID method = jEnv->GetStaticMethodID(cls, "sendCommand", "(II)V");
	jEnv->CallStaticVoidMethod(cls, method, cmd, data);
	return 0;
}

int sendCommandObj(JavaVM* jvm, jint cmd, jobject obj) {
	LOG_INFO("Sending command object data");
	JNIEnv* jEnv;
	jvm->AttachCurrentThread(&jEnv, NULL);
	jclass cls = jEnv->FindClass("uk/co/armedpineapple/cth/SDLActivity");
	jmethodID method = jEnv->GetStaticMethodID(cls, "sendCommandObject", "(ILjava/lang/Object;)V");
	jEnv->CallStaticVoidMethod(cls, method, cmd, obj);
	return 0;
}

int sendCommand(JavaVM* jvm, jint cmd) {
	LOG_INFO("Sending command");
	JNIEnv* jEnv;
	jvm->AttachCurrentThread(&jEnv, NULL);
	jclass cls = jEnv->FindClass("uk/co/armedpineapple/cth/SDLActivity");
	jmethodID method = jEnv->GetStaticMethodID(cls, "sendCommand", "(I)V");
	jEnv->CallStaticVoidMethod(cls, method, cmd);
	return 0;
}
