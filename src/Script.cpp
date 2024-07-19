#include "script.h"

lua::Script::Script()
	: L(NULL)
	, enabled(false)
{
}

lua::Script::~Script()
{
	reset();
}

bool lua::Script::Initialize(const char* path, std::function<void(lua_State* L)> register_extensions)
{
	if (L != NULL)
	{
		this->reset();
	}
	L = luaL_newstate();
	luaL_openlibs(L);

	enabled = luaL_dofile(L, path) == LUA_OK;
	if (enabled)
	{
		register_extensions(L);
	}
	else
	{
		printf("%s could not be initialized:\n>\t%s\n", path, lua_tostring(L, -1));
	}

	return enabled;
}

bool lua::Script::run(const char* functionName)
{
	if (!enabled) return false;
	return lua::execute_function(this->L, functionName);
}

void lua::Script::reset()
{
	if (L != NULL)
	{
		lua_close(L);
		L = NULL;
		enabled = false;
	}
}

bool lua::execute_function(lua_State* L, const char* functionName)
{
	try {
		luabridge::LuaRef func = luabridge::getGlobal(L, functionName);
		func();
		return true;
	}
	catch (luabridge::LuaException const& e)
	{
		lua::capture_exception(e, functionName);
	}
	return false;
}

void lua::capture_exception(const luabridge::LuaException& e, const char* functionName)
{
	TraceLog(TraceLogLevel::LOG_ERROR, "[LUA::%s]\n >\t%s", functionName, e.what());
}
