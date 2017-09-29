#include "Logger.h"

// LuaScript::unlua_get

template<typename T>
T LuaScript::unlua_get(const std::string& variable_name) {
    if(!lua_state) {
        Log(ERROR) << "Script is not loaded! [" << variable_name << "]";
        return unlua_getDefault<T>();
    }

    T result;
    if(unlua_getToStack(variable_name)) { // variable succesfully on top of stack
        result = unlua_getValue<T>(variable_name);  
    }
    else {
        result = unlua_getDefault<T>();
    }

    unlua_clean();
    return result;
}

// LuaScript::unlua_getDefault

template<typename T>
T LuaScript::unlua_getDefault() {
	return 0;
}

template<>
inline std::string LuaScript::unlua_getDefault<std::string>() {
	return "null";
}


// LuaScript::unlua_getValue

template<typename T>
T LuaScript::unlua_getValue(const std::string& variable_name) {
    return 0;
}

template <> 
inline bool LuaScript::unlua_getValue<bool>(const std::string& variable_name) {
	((void)(variable_name));
	return (bool)lua_toboolean(lua_state, -1);
}

template <> 
inline double LuaScript::unlua_getValue<double>(const std::string& variable_name) {
	if(!lua_isnumber(lua_state, -1)) {
		Log(WARN) << "'" << variable_name << "' is not a number!";
	}
	return (double)lua_tonumber(lua_state, -1);
}

template <>
inline int LuaScript::unlua_getValue<int>(const std::string& variable_name) {
	if(!lua_isnumber(lua_state, -1)) {
		Log(WARN) << "'" << variable_name << "' is not a number!";
	}
	return (int)lua_tonumber(lua_state, -1);
}

template <>
inline std::string LuaScript::unlua_getValue<std::string>(const std::string& variable_name) {
	std::string s = "null";
	if(lua_isstring(lua_state, -1)) {
		s = std::string(lua_tostring(lua_state, -1));
	} else {
		Log(DEBUG) << "'" << variable_name << "' is not a string!";
	}
	return s;
}
