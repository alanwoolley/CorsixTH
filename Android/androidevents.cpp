#include "androidevents.h"

#include <cstring>
#include <cstdio>
#include "lua_sdl.h"
#include "th_lua.h"

static void l_pushtablestring(lua_State *L, const char* k, char* v) {
    lua_pushstring(L, k);
    lua_pushstring(L, v);
    lua_settable(L, -3);
}

static void l_pushtablebool(lua_State *L, const char* k, unsigned char v) {
    lua_pushstring(L, k);
    lua_pushboolean(L, (int) v);
    lua_settable(L, -3);
}

static void l_pushtableint(lua_State *L, const char* k, int v) {
    lua_pushstring(L, k);
    lua_pushinteger(L, v);
    lua_settable(L, -3);
}

int handleAndroidEvents(SDL_Event &e, lua_State* dispatcher) {
    int nargs = -1;
    char buf[255];
    char d[255];

    switch (e.type) {
        case SDL_FINGERDOWN:
            lua_pushliteral(dispatcher, "touchdown");
            lua_pushinteger(dispatcher, e.tfinger.fingerId);
            lua_pushinteger(dispatcher, e.tfinger.x);
            lua_pushinteger(dispatcher, e.tfinger.y);
            nargs = 4;
            break;
        case SDL_FINGERUP:
            lua_pushliteral(dispatcher, "touchup");
            lua_pushinteger(dispatcher, e.tfinger.fingerId);
            lua_pushinteger(dispatcher, e.tfinger.x);
            lua_pushinteger(dispatcher, e.tfinger.y);
            nargs = 4;
            break;
        case SDL_FINGERMOTION:
            lua_pushliteral(dispatcher, "touchmove");
            lua_pushinteger(dispatcher, e.tfinger.fingerId);
            lua_pushinteger(dispatcher, e.tfinger.x);
            lua_pushinteger(dispatcher, e.tfinger.y);
            lua_pushinteger(dispatcher, e.tfinger.dx);
            lua_pushinteger(dispatcher, e.tfinger.dy);
            nargs = 6;
            break;
        case SDL_USEREVENT_LOAD:
            lua_pushliteral(dispatcher, "load");
            strcpy(d, (const char*) e.user.data1);
            sprintf(buf, "Loading %s", d);
            lua_pushstring(dispatcher, (const char*) d);
            nargs = 2;
            break;
        case SDL_USEREVENT_SAVE:
            lua_pushliteral(dispatcher, "save");
            strcpy(d, (const char*) e.user.data1);
            sprintf(buf, "Saving %s", d);
            lua_pushstring(dispatcher, (const char*) d);
            nargs = 2;
            break;
        case SDL_USEREVENT_GAMESPEED:
            lua_pushliteral(dispatcher, "gamespeed");
            strcpy(d, (const char*) e.user.data1);
            sprintf(buf, "Game speed: %s", d);
            lua_pushstring(dispatcher, (const char*) d);
            nargs = 2;
            break;
        case SDL_USEREVENT_RESTART:
            lua_pushliteral(dispatcher, "restart");
            nargs = 1;
            break;
        case SDL_USEREVENT_AUTOSAVE:
            lua_pushliteral(dispatcher, "tryautosave");
            strcpy(d, (const char*) e.user.data1);
            lua_pushstring(dispatcher, (const char*) d);
            nargs = 2;
            break;

        case SDL_USEREVENT_CONFIGURATION: {
            lua_pushliteral(dispatcher, "configupdate");
            Configuration *newConfig = (Configuration *) e.user.data1;
            lua_newtable(dispatcher);
            l_pushtablestring(dispatcher, "originalFilesPath",
                              newConfig->originalFilesPath);
            l_pushtablestring(dispatcher, "cthPath", newConfig->cthPath);
            l_pushtablestring(dispatcher, "language", newConfig->language);

            l_pushtableint(dispatcher, "fpsLimit", newConfig->fpsLimit);
            l_pushtableint(dispatcher, "edgeScrollSize",
                           newConfig->edgeScrollSize);
            l_pushtableint(dispatcher, "edgeScrollSpeed",
                           newConfig->edgeScrollSpeed);
            l_pushtableint(dispatcher, "controlsMode",
                           newConfig->controlsMode);
            l_pushtablebool(dispatcher, "edgeScroll",
                            newConfig->edgeScroll);
            l_pushtablebool(dispatcher, "playSoundFx",
                            newConfig->playSoundFx);
            l_pushtablebool(dispatcher, "playMusic", newConfig->playMusic);
            l_pushtablebool(dispatcher, "playAnnouncements",
                            newConfig->playAnnouncements);
            l_pushtablebool(dispatcher, "adviserEnabled",
                            newConfig->adviserEnabled);

            // Update FPS Limit
            set_fps_limit(newConfig->fpsLimit);

            nargs = 2;
        }
            break;
        case SDL_USEREVENT_SHOWCHEATS:
            lua_pushliteral(dispatcher, "showcheats");
            nargs = 1;
            break;
        case SDL_USEREVENT_SHOWJUKEBOX:
            lua_pushliteral(dispatcher, "showjukebox");
            nargs = 1;
            break;
    }

    return nargs;
}