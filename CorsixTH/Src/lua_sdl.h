/*
Copyright (c) 2009 Peter "Corsix" Cawley

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef CORSIX_TH_LUA_SDL_H_
#define CORSIX_TH_LUA_SDL_H_

#include "lua.hpp"
#include <SDL.h>

// SDL event codes used for delivering custom events to l_mainloop in
// sdl_core.cpp
// SDL_USEREVENT_TICK - informs script of a timer tick
#define SDL_USEREVENT_TICK (SDL_USEREVENT + 0)
// SDL_USEREVENT_MUSIC_OVER - informs script of SDL_Mixer music finishing
#define SDL_USEREVENT_MUSIC_OVER (SDL_USEREVENT + 1)
// SDL_USEREVENT_MUSIC_LOADED - informs script that async music is loaded
#define SDL_USEREVENT_MUSIC_LOADED (SDL_USEREVENT + 2)
// SDL USEREVENT_MOVIE_OVER - informs script of THMovie movie finishing
#define SDL_USEREVENT_MOVIE_OVER (SDL_USEREVENT + 3)
// SDL_USEREVENT_SOUND_OVER - informs script of a played sound finishing.
#define SDL_USEREVENT_SOUND_OVER (SDL_USEREVENT + 4)

// Alan's Android Stuff
#define SDL_USEREVENT_LOAD (SDL_USEREVENT + 5)
#define SDL_USEREVENT_SAVE (SDL_USEREVENT + 6)
#define SDL_USEREVENT_RESTART (SDL_USEREVENT + 7)
#define SDL_USEREVENT_GAMESPEED (SDL_USEREVENT + 8)
#define SDL_USEREVENT_AUTOSAVE (SDL_USEREVENT + 9)
#define SDL_USEREVENT_CONFIGURATION (SDL_USEREVENT + 10)
#define SDL_USEREVENT_SHOWCHEATS (SDL_USEREVENT + 11)
#define SDL_USEREVENT_FF_REFRESH (SDL_USEREVENT + 12)
#define SDL_USEREVENT_SHOWJUKEBOX (SDL_USEREVENT + 13)

int luaopen_sdl(lua_State *L);

void set_fps_limit(int fps);

typedef struct {
	int fpsLimit, edgeScrollSize, edgeScrollSpeed;
	enum controls_mode {
		NORMAL = 1, DESKTOP = 2, TOUCHPAD = 3
	} controlsMode;
	unsigned char playSoundFx, playMusic, playAnnouncements, adviserEnabled,
			edgeScroll, vibrate;
	char* originalFilesPath, *cthPath, *language;
} Configuration;

int l_load_music_async_callback(lua_State *L);


#endif // CORSIX_TH_LUA_SDL_H_
