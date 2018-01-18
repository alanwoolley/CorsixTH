#ifndef COMMANDS_H_
#define COMMANDS_H_

#ifndef NULL
#define NULL 0
#endif

#include <jni.h>

#define COMMAND_SHOW_MENU 0
#define COMMAND_SHOW_LOAD_DIALOG 6
#define COMMAND_SHOW_SAVE_DIALOG 7
#define COMMAND_RESTART_GAME 8
#define COMMAND_QUICK_LOAD 9
#define COMMAND_QUICK_SAVE 10
#define COMMAND_SHOW_KEYBOARD 11
#define COMMAND_HIDE_KEYBOARD 12
#define COMMAND_SHOW_ABOUT_DIALOG 13
#define COMMAND_PAUSE_GAME 14
#define COMMAND_SHOW_SETTINGS_DIALOG 15
#define COMMAND_GAME_SPEED_UPDATED 16
#define COMMAND_GAME_LOAD_ERROR 17
#define COMMAND_HIDE_MENU 18
#define COMMAND_START_VIBRATION 19
#define COMMAND_STOP_VIBRATION 20
#define COMMAND_GAME_SAVE_UPDATED 21

int sendCommandInt(JavaVM* jvm, jint cmd, jint data);
int sendCommand(JavaVM* jvm, jint cmd);
int sendCommandObj(JavaVM* jvm, jint cmd, jobject obj);

#endif