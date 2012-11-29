/**
* Copyright (c) 2006-2012 LOVE Development Team
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

// Libraries.
#include "libraries/luasocket/luasocket.h"

// Scripts
#include "scripts/boot.lua.h"
#include "scripts/android.lua.h"

#include "loveLog.h"
extern void sendAndroidDisableSensor(const char *str);
extern void sendAndroidEnableSensor(const char *str);

#endif // LOVE_BUILD_EXE

#ifdef LOVE_BUILD_STANDALONE

// All modules define a c-accessible luaopen
// so let's make use of those, instead
// of addressing implementations directly.
extern "C"
{
	extern int luaopen_love_audio(lua_State*);
	extern int luaopen_love_event(lua_State*);
	extern int luaopen_love_filesystem(lua_State*);
	extern int luaopen_love_font(lua_State*);
	extern int luaopen_love_graphics(lua_State*);
	extern int luaopen_love_image(lua_State*);
	extern int luaopen_love_joystick(lua_State*);
	extern int luaopen_love_keyboard(lua_State*);
	extern int luaopen_love_mouse(lua_State*);
	extern int luaopen_love_physics(lua_State*);
	extern int luaopen_love_sound(lua_State*);
	extern int luaopen_love_timer(lua_State*);
	extern int luaopen_love_thread(lua_State*);
}

static const luaL_Reg modules[] = {
	{ "love.audio", luaopen_love_audio },
	{ "love.event", luaopen_love_event },
	{ "love.filesystem", luaopen_love_filesystem },
	{ "love.font", luaopen_love_font },
	{ "love.graphics", luaopen_love_graphics },
	{ "love.image", luaopen_love_image },
// 	{ "love.joystick", luaopen_love_joystick },
	{ "love.keyboard", luaopen_love_keyboard },
	{ "love.mouse", luaopen_love_mouse },
	{ "love.physics", luaopen_love_physics },
	{ "love.sound", luaopen_love_sound },
	{ "love.timer", luaopen_love_timer },
	{ "love.thread", luaopen_love_thread },
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
	lua_pushstring(L, love::VERSION);
	lua_setfield(L, -2, "_version");

	lua_pushnumber(L, love::VERSION_MAJOR);
	lua_setfield(L, -2, "_version_major");
	lua_pushnumber(L, love::VERSION_MINOR);
	lua_setfield(L, -2, "_version_minor");
	lua_pushnumber(L, love::VERSION_REV);
	lua_setfield(L, -2, "_version_revision");

	lua_pushstring(L, love::VERSION_CODENAME);
	lua_setfield(L, -2, "_version_codename");

#ifdef LOVE_LEGENDARY_CONSOLE_IO_HACK
	lua_pushcfunction(L, w__openConsole);
	lua_setfield(L, -2, "_openConsole");
#endif // LOVE_LEGENDARY_CONSOLE_IO_HACK

	lua_newtable(L);

	for (int i = 0; love::VERSION_COMPATIBILITY[i] != 0; ++i)
	{
		lua_pushstring(L, love::VERSION_COMPATIBILITY[i]);
		lua_rawseti(L, -2, i+1);
	}

	lua_setfield(L, -2, "_version_compat");
	
#ifdef LOVE_WINDOWS
	lua_pushstring(L, "Windows");
#elif defined(LOVE_MACOSX)
	lua_pushstring(L, "OS X");
#elif defined(LOVE_LINUX)
	lua_pushstring(L, "Linux");
#else
	lua_pushstring(L, "Unknown");
#endif
	lua_setfield(L, -2, "_os");

#ifdef LOVE_BUILD_STANDALONE

	// Preload module loaders.
	for (int i = 0; modules[i].name != 0; i++)
	{
		love::luax_preload(L, modules[i].func, modules[i].name);
	}

	love::luasocket::__open(L);

#endif // LOVE_BUILD_STANDALONE

	return 1;
}

#ifdef LOVE_LEGENDARY_CONSOLE_IO_HACK

int w__openConsole(lua_State * L)
{
	static bool is_open = false;

	if (is_open)
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

#ifdef LOVE_LEGENDARY_LIBSTDCXX_HACK

// Workarounds for symbols that are missing from Leopard stdlibc++.dylib.
// http://stackoverflow.com/questions/3484043/os-x-program-runs-on-dev-machine-crashing-horribly-on-others
_GLIBCXX_BEGIN_NAMESPACE(std)
// From ostream_insert.h
template ostream& __ostream_insert(ostream&, const char*, streamsize);

#ifdef _GLIBCXX_USE_WCHAR_T
template wostream& __ostream_insert(wostream&, const wchar_t*, streamsize);
#endif

// From ostream.tcc
template ostream& ostream::_M_insert(long);
template ostream& ostream::_M_insert(unsigned long);
template ostream& ostream::_M_insert(bool);
#ifdef _GLIBCXX_USE_LONG_LONG
template ostream& ostream::_M_insert(long long);
template ostream& ostream::_M_insert(unsigned long long);
#endif
template ostream& ostream::_M_insert(double);
template ostream& ostream::_M_insert(long double);
template ostream& ostream::_M_insert(const void*);

#ifdef _GLIBCXX_USE_WCHAR_T
template wostream& wostream::_M_insert(long);
template wostream& wostream::_M_insert(unsigned long);
template wostream& wostream::_M_insert(bool);
#ifdef _GLIBCXX_USE_LONG_LONG
template wostream& wostream::_M_insert(long long);
template wostream& wostream::_M_insert(unsigned long long);
#endif
template wostream& wostream::_M_insert(double);
template wostream& wostream::_M_insert(long double);
template wostream& wostream::_M_insert(const void*);
#endif

_GLIBCXX_END_NAMESPACE

#endif // LOVE_LEGENDARY_LIBSTDCXX_HACK

#include <sstream>

template<class T> std::string toString(T t)
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
	if (argc > 1 && strcmp(argv[1],"--version") == 0)
	{
		printf("LOVE %s (%s)\n", love::VERSION, love::VERSION_CODENAME);
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

		if (argc > 0)
		{
			lua_pushstring(L, argv[0]);
			lua_rawseti(L, -2, -2);
		}

		lua_pushstring(L, "embedded boot.lua");
		lua_rawseti(L, -2, -1);

		for (int i = 1; i<argc; i++)
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
	if (L == 0)
	  return 0;
	
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
