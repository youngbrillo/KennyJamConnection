#pragma once
#include <lua.hpp>
#include <string>
#include <functional>
#include <raylib/raylib.h>
#include <LuaBridge/LuaBridge.h>

namespace lua
{
	class Script
	{
	public:
		Script();
		~Script();

		bool Initialize(const char* path, std::function<void(lua_State* L)> extensions);

		bool run(const char* functionName);

		template<typename  T>
		inline bool run(const char* functionName, T* v);

		template<typename T, typename U>
		inline bool run(const char* functionName, T* v, U* v1);

		template<typename T, typename U, typename V>
		inline bool run(const char* functionName, T* v, U* v1, V* v2);

		void reset();
		lua_State* getState() const { return L; };
		bool Enabled() const { return this->enabled; }
	protected:
		lua_State* L;
		bool enabled = false;
	};


	bool execute_function(lua_State* L, const char* functionName);
	void capture_exception(const luabridge::LuaException& e, const char* functionName);
};



namespace lua
{
	template <typename T>
	inline bool execute_function(lua_State* L, const char* functionName, T* v1)
	{
		if (L == NULL) return false;
		try {
			luabridge::LuaRef func = luabridge::getGlobal(L, functionName);
			func(v1);
			return true;
		}
		catch (luabridge::LuaException const& e)
		{
			lua::capture_exception(e, functionName);
			return false;
		}
	}

	template<typename T, typename U>
	inline bool execute_function(lua_State* L, const char* functionName, T* v1, U* v2)
	{
		if (L == NULL) return false;
		try {
			luabridge::LuaRef func = luabridge::getGlobal(L, functionName);
			func(v1, v2);
			return true;
		}
		catch (luabridge::LuaException const& e)
		{
			lua::capture_exception(e, functionName);
			return false;
		}
	}

	template<typename T, typename U, typename V>
	inline bool execute_function(lua_State* L, const char* functionName, T* v1, U* v2, V* v3)
	{
		if (L == NULL) return false;

		try {
			luabridge::LuaRef func = luabridge::getGlobal(L, functionName);
			func(v1, v2, v3);
			return true;
		}
		catch (luabridge::LuaException const& e)
		{
			lua::capture_exception(e, functionName);
			return false;
		}
	}
}
template<typename T>
inline bool lua::Script::run(const char* functionName, T* v)
{
	if (!enabled) return false;
	return lua::execute_function(this->L, functionName, v);
}
template<typename T, typename U>
inline bool lua::Script::run(const char* functionName, T* v, U* v1)
{
	if (!enabled) return false;
	return lua::execute_function(this->L, functionName, v, v1);
}
template<typename T, typename U, typename V>
inline bool lua::Script::run(const char* functionName, T* v, U* v1, V* v2)
{
	if (!enabled) return false;
	return lua::execute_function(this->L, functionName, v, v1, v2);
}
