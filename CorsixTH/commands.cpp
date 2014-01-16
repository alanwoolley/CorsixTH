#include "commands.h"


int sendCommandInt(JavaVM* jvm, jint cmd, jint data) {
	JNIEnv* jEnv;
	jvm->AttachCurrentThread(&jEnv, NULL);
	jclass cls = jEnv->FindClass("uk/co/armedpineapple/cth/SDLActivity");
	jmethodID method = jEnv->GetStaticMethodID(cls, "sendCommand", "(II)V");
	jEnv->CallStaticVoidMethod(cls, method, cmd, data);
	return 0;
}

int sendCommand(JavaVM* jvm, jint cmd) {
	JNIEnv* jEnv;
	jvm->AttachCurrentThread(&jEnv, NULL);
	jclass cls = jEnv->FindClass("uk/co/armedpineapple/cth/SDLActivity");
	jmethodID method = jEnv->GetStaticMethodID(cls, "sendCommand", "(I)V");
	jEnv->CallStaticVoidMethod(cls, method, cmd);
	return 0;
}
