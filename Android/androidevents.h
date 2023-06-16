#ifndef USEREVENTS_H_
#define USEREVENTS_H_

#include <SDL_events.h>
#include <lua_sdl.h>

// Custom user events for Android
// The value of these is given relative to the predefined CorsixTH user events. See lua_sdl.h.
#define CTH_USEREVENT_COUNT 4
#define ANDROID_USEREVENTS (SDL_USEREVENT + CTH_USEREVENT_COUNT)

#define SDL_USEREVENT_LOAD (ANDROID_USEREVENTS + 1)
#define SDL_USEREVENT_SAVE (ANDROID_USEREVENTS + 2)
#define SDL_USEREVENT_RESTART (ANDROID_USEREVENTS + 3)
#define SDL_USEREVENT_GAMESPEED (ANDROID_USEREVENTS + 4)
#define SDL_USEREVENT_AUTOSAVE (ANDROID_USEREVENTS + 5)
#define SDL_USEREVENT_CONFIGURATION (ANDROID_USEREVENTS + 6)
#define SDL_USEREVENT_SHOWCHEATS (ANDROID_USEREVENTS + 7)
#define SDL_USEREVENT_SHOWJUKEBOX (ANDROID_USEREVENTS + 8)

int handleAndroidEvents(SDL_Event &e, lua_State* dispatcher);

#endif
