#ifndef LOGGING_H_
#define LOGGING_H_
#include <android/log.h>
#include <string>

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "CorsixTH Native", "%s", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_ERROR, "CorsixTH Native", "%s", __VA_ARGS__))

void LOG_ERROR(std::string strtext);
void LOG_INFO(std::string strtext);
void START_LOGGING(const char* root);

#endif
