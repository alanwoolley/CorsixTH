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

#ifndef CORSIX_TH_CONFIG_H_
#define CORSIX_TH_CONFIG_H_

/** Interpreter script searching options **/
// Whether to search in some possible local dirs according to a fixed list,
// relative to current working dir or program dir
#define CORSIX_TH_SEARCH_LOCAL_DATADIRS

// Name of the script, usually 'CorsixTH.lua'
#define CORSIX_TH_INTERPRETER_NAME "CorsixTH.lua"

// A final path (with file name) to search
#define CORSIX_TH_INTERPRETER_PATH "CorsixTH/CorsixTH.lua"

/** Windows Platform SDK usage **/
// When compiling on Windows, the platform SDK should be used. However, when
// using the SDL rendering engine, the platform SDK is not used for anything
// critical, and so its use can be avoided if necessary.
#ifdef _WIN32
#define CORSIX_TH_USE_WIN32_SDK
#endif

/** Rendering engine choice **/
// SDL - Multiplatform, but suboptimal on some platforms
// DirectX 9 - Windows only, but always has HW accellerated (alpha) blitting
// OpenGL - Faster than SDL on supported platforms, but DX9 may still be
//          preferable on Windows (not yet finished, but should work).
#ifndef CORSIX_TH_RENDERING_ENGINE_DEFINED
#define CORSIX_TH_USE_SDL_RENDERER
#endif //CORSIX_TH_RENDERING_ENGINE_DEFINED

// When OpenGL is used as the rendering engine, try to make use of the
// extension which allows for rendering to a texture. This must be enabled to
// allow the zooming feature to work.
#define CORSIX_TH_USE_OGL_RENDER_TO_TEXTURE

/** Windows Platform SDK usage **/
// When compiling on Windows, the platform SDK should be used. However, when
// using the SDL rendering engine, the platform SDK is not used for anything
// critical, and so its use can be avoided if necessary.
#ifdef _WIN32
#define CORSIX_TH_USE_WIN32_SDK
#endif

/** SDL options **/
// On Windows, the default is to use the copy of SDLmain which is included with
// CorsixTH (SDL_main_win32.c), but a prebuilt SDLmain library can be used
// instead.
#ifdef CORSIX_TH_USE_WIN32_SDK
#define CORSIX_TH_USE_INCLUDED_SDL_MAIN
#endif

/** Audio options **/
// SDL_mixer is used for ingame audio. If this library is not present on your
// system, then you can comment out the next line and the game will not have
// any music.
#define CORSIX_TH_USE_SDL_MIXER

/** Movie options **/
// FFmpeg
// If this library is not present on your system, then you can comment out the
// next line and the game will not have movies.
#define CORSIX_TH_USE_FFMPEG

/** Font options **/
// FreeType2 can be used for font support beyond the CP437 bitmap fonts which
// come with Theme Hospital. It must be used if translations like Russian or
// Chinese are desired.
#define CORSIX_TH_USE_FREETYPE2

#ifdef _MSC_VER
#define CORSIX_TH_USE_PACK_PRAGMAS 1
#define CORSIX_TH_PACKED_FLAGS
#else
#define CORSIX_TH_USE_PACK_PRAGMAS 0
#define CORSIX_TH_PACKED_FLAGS __attribute__((__packed__))
#endif

/** Standard includes **/
#include <cstddef>
#include <cstdint>

// We bring in the most common stddef and stdint types to avoid typing
// clang-format off
using std::int8_t;
using std::int16_t;
using std::int32_t;
using std::int64_t;
using std::size_t;
using std::uint8_t;
using std::uint16_t;
using std::uint32_t;
using std::uint64_t;
// clang-format on

/** Visual Leak Detector **/
// In Visual Studio, Visual Leak Detector can be used to find memory leaks.
//#define CORSIX_TH_USE_VLD

/** Report operating system **/
#define CORSIX_TH_OS "linux"

/** Report system architecture **/
#define CORSIX_TH_ARCH ""


#endif // CORSIX_TH_CONFIG_H_
