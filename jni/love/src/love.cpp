/**
* Copyright (c) 2006-2011 LOVE Development Team
*
* This software is provided 'as-is', without any express or implied
* warranty.  In no event will the authors be held liable for any damages
* arising from the use of this software.
*
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
*
* 1. The origin of this software must not be misrepresented; you must not
*    claim that you wrote the original software. If you use this software
*    in a product, an acknowledgment in the product documentation would be
*    appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must not be
*    misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
**/

// LOVE
#include <common/config.h>
#include <common/version.h>
#include <common/runtime.h>
#include <common/MemoryData.h>

#ifdef LOVE_WINDOWS
#include <windows.h>
#endif // LOVE_WINDOWS

#ifdef LOVE_LEGENDARY_CONSOLE_IO_HACK
#include <fcntl.h>
#include <io.h>
#include <iostream>
#include <fstream>
#endif // LOVE_LEGENDARY_CONSOLE_IO_HACK

#ifdef LOVE_BUILD_EXE

// SDL
// #include <SDL.h>

// Modules
#include <audio/wrap_Audio.h>
#include <event/sdl/wrap_Event.h>
#include <filesystem/physfs/wrap_Filesystem.h>
#include <font/freetype/wrap_Font.h>
#include <graphics/opengl/wrap_Graphics.h>
#include <image/wrap_Image.h>
//#include <joystick/sdl/wrap_Joystick.h>
#include <keyboard/sdl/wrap_Keyboard.h>
#include <mouse/sdl/wrap_Mouse.h>
#include <physics/box2d/wrap_Physics.h>
#include <sound/wrap_Sound.h>
#include <timer/sdl/wrap_Timer.h>
#include <thread/sdl/wrap_Thread.h>

// Libraries.
#include "libraries/luasocket/luasocket.h"

// Scripts
#include "scripts/boot.lua.h"
#include "scripts/android.lua.h"

// Android specific stuff
#include "loveLog.h"
extern void sendAndroidDisableSensor(const char *str);
extern void sendAndroidEnableSensor(const char *str);

#endif // LOVE_BUILD_EXE

#ifdef LOVE_BUILD_STANDALONE

static const luaL_Reg modules[] = {
	{ "love.audio", love::audio::luaopen_love_audio },
	{ "love.event", love::event::sdl::luaopen_love_event },
	{ "love.filesystem", love::filesystem::physfs::luaopen_love_filesystem },
	{ "love.font", love::font::freetype::luaopen_love_font },
	{ "love.graphics", love::graphics::opengl::luaopen_love_graphics },
	{ "love.image", love::image::luaopen_love_image },
//	{ "love.joystick", love::joystick::sdl::luaopen_love_joystick },
	{ "love.keyboard", love::keyboard::sdl::luaopen_love_keyboard },
	{ "love.mouse", love::mouse::sdl::luaopen_love_mouse },
	{ "love.physics", love::physics::box2d::luaopen_love_physics },
	{ "love.sound", love::sound::luaopen_love_sound },
	{ "love.timer", love::timer::sdl::luaopen_love_timer },
	{ "love.thread", love::thread::sdl::luaopen_love_thread },
	{ 0, 0 }
};

#endif // LOVE_BUILD_STANDALONE

#ifdef LOVE_LEGENDARY_CONSOLE_IO_HACK
int w__openConsole(lua_State * L);
#endif // LOVE_LEGENDARY_CONSOLE_IO_HACK

extern "C" LOVE_EXPORT int luaopen_love(lua_State * L)
{
	love::luax_insistglobal(L, "love");

	// Set version information.
	lua_pushinteger(L, love::VERSION);
	lua_setfield(L, -2, "_version");

	lua_pushstring(L, love::VERSION_STR);
	lua_setfield(L, -2, "_version_string");

	lua_pushstring(L, love::VERSION_CODENAME);
	lua_setfield(L, -2, "_version_codename");

#ifdef LOVE_LEGENDARY_CONSOLE_IO_HACK
	lua_pushcfunction(L, w__openConsole);
	lua_setfield(L, -2, "_openConsole");
#endif // LOVE_LEGENDARY_CONSOLE_IO_HACK

	lua_newtable(L);

	for(int i = 0; love::VERSION_COMPATIBILITY[i] != 0; ++i)
	{
		lua_pushinteger(L, love::VERSION_COMPATIBILITY[i]);
		lua_rawseti(L, -2, i+1);
	}

	lua_setfield(L, -2, "_version_compat");

	lua_pop(L, 1); // love

#ifdef LOVE_BUILD_STANDALONE

	// Preload module loaders.
	for(int i = 0; modules[i].name != 0; i++)
	{
		love::luax_preload(L, modules[i].func, modules[i].name);
	}

	love::luasocket::__open(L);

#endif // LOVE_BUILD_STANDALONE

	return 0;
}

#ifdef LOVE_LEGENDARY_CONSOLE_IO_HACK

int w__openConsole(lua_State * L)
{
	static bool is_open = false;

	if(is_open)
		return 0;

	static const int MAX_CONSOLE_LINES = 5000;
	long std_handle;
	int console_handle;
	CONSOLE_SCREEN_BUFFER_INFO console_info;
	FILE *fp;

	AllocConsole();

	// Set size.
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &console_info);
	console_info.dwSize.Y = MAX_CONSOLE_LINES;
	SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), console_info.dwSize);

	SetConsoleTitle(TEXT("LOVE Console"));

	// Redirect stdout.
	std_handle = (long)GetStdHandle(STD_OUTPUT_HANDLE);
	console_handle = _open_osfhandle(std_handle, _O_TEXT);
	fp = _fdopen(console_handle, "w");
	*stdout = *fp;
	setvbuf(stdout, NULL, _IONBF, 0);

	// Redirect stdin.
	std_handle = (long)GetStdHandle(STD_INPUT_HANDLE);
	console_handle = _open_osfhandle(std_handle, _O_TEXT);
	fp = _fdopen(console_handle, "r");
	*stdin = *fp;
	setvbuf(stdin, NULL, _IONBF, 0);

	// Redirect stderr.
	std_handle = (long)GetStdHandle(STD_ERROR_HANDLE);
	console_handle = _open_osfhandle(std_handle, _O_TEXT);
	fp = _fdopen(console_handle, "w");
	*stderr = *fp;
	setvbuf(stderr, NULL, _IONBF, 0);

	// Sync std::cout, std::cerr, etc.
	std::ios::sync_with_stdio();

	is_open = true;

	return 0;
}

#endif // LOVE_LEGENDARY_CONSOLE_IO_HACK


#include <sstream>

template<class T> std::string toString(T t)
{
	std::string s;
	std::stringstream out;
	out << t;
	s = out.str();
	return s;
}


static int android_print (lua_State *L)
{
	std::string s("lua print: ");

	int n = lua_gettop(L);

	for (int i = 1; i <= n; i++)
	{
		if (i > 1)
		{
			s.append(", ");
		}

		if (lua_isstring(L, i))
		{
			s.append(luaL_checkstring(L, i));
		}
		else if (lua_isnumber(L, i))
		{
			s.append(toString(luaL_checknumber(L, i)));
		}
		else if (lua_isnil(L, i))
		{
			s.append("NIL");
		}
		else if (lua_istable(L, i))
		{
			s.append("{...}");
		}
		else if (lua_isboolean(L, i))
		{
			s.append(toString(lua_toboolean(L, i) ? "true" : "false"));
		}
		else
		{
			s.append("?");
		}
	}

	LOGI(s.c_str());

	return 0;
}

static int android_sensor_enable (lua_State *L)
{
	int n = lua_gettop(L);
	if(n < 1)
	{
		LOGE("Don't u wanna say which sensor?");
		return 1;
	}

	// just ignore if it is not a string
	if(lua_isstring(L, 1))
	{
		const char *str = luaL_checkstring(L, 1);
		sendAndroidEnableSensor(str);
	}
	return 0;
}

static int android_sensor_disable (lua_State *L)
{
	int n = lua_gettop(L);
	if(n < 1)
	{
		LOGE("Don't u wanna say which sensor?");
		return 1;
	}

	// just ignore if it is not a string
	if(lua_isstring(L, 1))
	{
		const char *str = luaL_checkstring(L, 1);
		sendAndroidDisableSensor(str);
	}
	return 0;
}

void android_prepare(lua_State * L)
{
	lua_pushcfunction(L, android_print);
	lua_setglobal(L, "print");
	
	lua_pushcfunction(L, android_sensor_enable);
	lua_setglobal(L, "love_sensor_enable");
	
	lua_pushcfunction(L, android_sensor_disable);
	lua_setglobal(L, "love_sensor_disable");

	if (luaL_loadbuffer(L, (const char *)love::android_lua, sizeof(love::android_lua), "android.lua") == 0)
				lua_call(L, 0, 0);
}

#ifdef LOVE_BUILD_EXE

lua_State * L = 0;

int main_prepare(int argc, char ** argv)
{
	// Oh, you just want the version? Okay!
	if(argc > 1 && strcmp(argv[1],"--version") == 0) {
		printf("LOVE %s (%s)\n", love::VERSION_STR, love::VERSION_CODENAME);
		return 0;
	}

	// Create the virtual machine.
	L = lua_open();
	luaL_openlibs(L);

	love::luax_preload(L, luaopen_love, "love");

	luaopen_love(L);

	// Add command line arguments to global arg (like stand-alone Lua).
	{
		lua_newtable(L);

		if(argc > 0)
		{
			lua_pushstring(L, argv[0]);
			lua_rawseti(L, -2, -2);
		}

		lua_pushstring(L, "embedded boot.lua");
		lua_rawseti(L, -2, -1);

		for(int i = 1; i<argc; i++)
		{
			lua_pushstring(L, argv[i]);
			lua_rawseti(L, -2, i);
		}

		lua_setglobal(L, "arg");
	}

	// Add love.__exe = true.
	// This indicates that we're running the
	// standalone version of love, and not the
	// DLL version.
	{
		lua_getglobal(L, "love");
		lua_pushboolean(L, 1);
		lua_setfield(L, -2, "_exe");
		lua_pop(L, 1);
	}

	// Setup android specific stuff
	android_prepare(L);

	// Boot
	if (luaL_loadbuffer(L, (const char *)love::boot_lua, sizeof(love::boot_lua), "boot.lua") == 0)
	{
		int result = lua_pcall(L, 0, 0, 0);
		if (result != 0)
		{
			LOGE(lua_tostring(L, -1));
			lua_pop(L, 1);
		}
	}

	return 0;
}

lua_State *main_getLuaState()
{
	return L;
}

int main_step()
{
	if (L == 0)return 0;

	{
		// call _love_update()
		lua_getfield(L, LUA_GLOBALSINDEX, "_love_update");
		int result = lua_pcall(L, 0, 0, 0);
		if (result != 0)
		{
			LOGE(lua_tostring(L, -1));
			lua_pop(L, 1);
		}
	}

	{
		// call _love_draw()
		lua_getfield(L, LUA_GLOBALSINDEX, "_love_draw");
		int result = lua_pcall(L, 0, 0, 0);
		if (result != 0)
		{
			LOGE(lua_tostring(L, -1));
			lua_pop(L, 1);
		}
	}

	return 0;
}

int main_shutdown()
{
	if (L == 0)return 0;

	lua_close(L);
	L = 0;

	return 0;
}

#endif // LOVE_BUILD_EXE
