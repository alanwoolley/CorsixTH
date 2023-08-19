#include <android/log.h>
#include <pthread.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <fcntl.h>
#include <jni.h>
#include <string>

static int stdoutPipe[2];
static int stderrPipe[2];
static pthread_t stdoutThread;
static pthread_t stderrThread;
static JavaVM *vm;
static jobject logger;
static jmethodID midInfo, midError, midDebug;

struct LoggingArgs {
    void (*logFunction)(std::string, JNIEnv *);
    int pipeFd;
};

static jbyteArray asByteArray(std::string message, JNIEnv *env) {
    jbyteArray array = env->NewByteArray(message.length());
    env->SetByteArrayRegion(array, 0, message.length(), (const jbyte *) message.c_str());
    return array;
}


static void info(std::string message, JNIEnv *env) {
    jbyteArray messageBytes = asByteArray(message, env);
    env->CallVoidMethod(logger, midInfo, messageBytes);
}

static void error(std::string message, JNIEnv *env) {
    jbyteArray messageBytes = asByteArray(message, env);
    env->CallVoidMethod(logger, midError, messageBytes);
}

static void debug(std::string message, JNIEnv *env) {
    jbyteArray messageBytes = asByteArray(message, env);
    env->CallVoidMethod(logger, midDebug, messageBytes);
}

static void *loggingFunction(void *arg) {
    JNIEnv *threadEnv;
    vm->AttachCurrentThread(&threadEnv, NULL);

    auto *args = static_cast<LoggingArgs *>(arg);

    ssize_t readSize;
    char buf[128];

    while ((readSize = read(args->pipeFd, buf, sizeof buf - 1)) > 0) {
        if (buf[readSize - 1] == '\n') {
            --readSize;
        }

        buf[readSize] = 0;  // add null-terminator

        args->logFunction(std::string(buf), threadEnv);
    }

    vm->DetachCurrentThread();
    return 0;
}

static int runLoggingThread() { // run this function to redirect your output to android log
    setvbuf(stdout, 0, _IOLBF, 0); // make stdout line-buffered
    setvbuf(stderr, 0, _IONBF, 0); // make stderr unbuffered

    pipe(stdoutPipe); // Create the stdout pipe
    pipe(stderrPipe); // Create the stderr pipe

    // Redirect stdout to the write end of stdoutPipe
    dup2(stdoutPipe[1], 1);

    // Redirect stderr to the write end of stderrPipe
    dup2(stderrPipe[1], 2);

    /* spawn the logging threads */
    auto* stdoutArgs = new LoggingArgs {&info, stdoutPipe[0]};
    pthread_create(&stdoutThread, 0, loggingFunction, (void *) stdoutArgs);
    pthread_detach(stdoutThread);

    auto* stderrArgs = new LoggingArgs {&error, stderrPipe[0]};
    pthread_create(&stderrThread, 0, loggingFunction, (void *) stderrArgs);
    pthread_detach(stderrThread);

    return 0;
}

extern "C" JNIEXPORT void JNICALL
Java_uk_co_armedpineapple_cth_GameActivity_startLogger(JNIEnv *env, jobject jobj) {

    env->GetJavaVM(&vm);
    jclass cls = env->FindClass("uk/co/armedpineapple/cth/NativeLogger");
    jmethodID methodID = env->GetMethodID(cls, "<init>", "()V");
    logger = env->NewObject(cls, methodID);
    logger = env->NewGlobalRef(logger);
    midInfo = env->GetMethodID(cls, "info", "([B)V");
    midError = env->GetMethodID(cls, "error", "([B)V");
    midDebug = env->GetMethodID(cls, "debug", "([B)V");

    runLoggingThread();
}