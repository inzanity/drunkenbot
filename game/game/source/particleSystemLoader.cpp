#include "stdafx.h"

extern "C"
{
#include "../../tools/lua/include/lua.h"
#include "../../tools/lua/include/lualib.h"
#include "../../tools/lua/include/lauxlib.h"
}

#include "../include/particleSystemLoader.h"

struct lua_State {};

char *CParticleSystemLoader::mErrorMsg = NULL;
CParticleSystem *CParticleSystemLoader::mParticleSystem = NULL;
const char *CParticleSystemLoader::mFileName = NULL;
int CParticleSystemLoader::mPosNum = 0;
int CParticleSystemLoader::mColorNum = 0;
int CParticleSystemLoader::mSizeNum = 0;

CParticleSystem *CParticleSystemLoader::load(const char *aFileName)
{
	mFileName = aFileName;
	mPosNum = mColorNum = mSizeNum = 0;
	int status;
	lua_State* state = lua_open();

	static const luaL_reg lualibs[] = 
	{
		{"base", luaopen_base},
		{"table", luaopen_table},
		{"io", luaopen_io},
		{"string", luaopen_string},
		{"math", luaopen_math},
		{"debug", luaopen_debug},
		{"loadlib", luaopen_loadlib}, 
		{NULL, NULL}
	};

	// load the list of libs
	const luaL_reg *lib = lualibs;
	for (; lib->func; lib++) 
	{
		lib->func(state); // open library
		lua_settop(state, 0); // discard any results
	}
	lua_register(state, "initParticleSystem", initParticleSystem);
	lua_register(state, "setParticle", setParticle);
	lua_register(state, "setDefaultColor", setDefaultColor);
	lua_register(state, "setDefaultSize", setDefaultSize);
	lua_register(state, "rgb", rgb);
	lua_register(state, "rgba", rgba);

	status = luaL_loadfile(state, aFileName);

	if (status == 0)
		status = lua_pcall(state, 0, LUA_MULTRET, 0);

	if (status != 0 && lua_isstring(state, -1))
	{
		if (mErrorMsg)
			delete [] mErrorMsg;
		mErrorMsg = new char[strlen(lua_tostring(state, -1)) + 1];
		strcpy(mErrorMsg, lua_tostring(state, -1));
		return NULL;
	}

	lua_close(state);
	return mParticleSystem;
}

const char *CParticleSystemLoader::getErrorMsg()
{
	return mErrorMsg;
}

void CParticleSystemLoader::checkType(lua_State *aState, int aIndex, int aType, const char *aFunc)
{
	char msg[128];
	if (lua_type(aState, aIndex) != aType)
	{
		int index = (aIndex > 0 ? aIndex : lua_gettop(aState) + aIndex + 1);
		sprintf(msg, "%s: Cannot convert parameter %d to %s", aFunc, index, lua_typename(aState, aType));
		lua_pushstring(aState, msg);
		lua_error(aState);
	}
}

int CParticleSystemLoader::getPosArray(lua_State *aState, int aIndex, const char *aFunc, D3DXVECTOR3 *&aArray)
{
	checkType(aState, aIndex, LUA_TTABLE, aFunc);
	int n = luaL_getn(aState, aIndex); // get size of table
	aArray = new D3DXVECTOR3[n];
	for (int i = 0; i < n; i++)
	{
		lua_rawgeti(aState, aIndex, i + 1);
		checkType(aState, -1, LUA_TTABLE, aFunc);
		char key[2] = "x";
		for (int j = 0; j < 3; j++)
		{
			lua_pushstring(aState, key);
			lua_rawget(aState, -2);
			checkType(aState, -1, LUA_TNUMBER, aFunc);
			aArray[i][j] = (float)lua_tonumber(aState, -1);
			lua_pop(aState, 1);
			key[0]++;
		}
		lua_pop(aState, 1);
	}
	return n;
}

int CParticleSystemLoader::getFloatArray(lua_State *aState, int aIndex, const char *aFunc, float *&aArray)
{
	checkType(aState, aIndex, LUA_TTABLE, aFunc);
	int n = luaL_getn(aState, aIndex); // get size of table
	aArray = new float[n];
	for (int i = 0; i < n; i++)
	{
		lua_rawgeti(aState, aIndex, i + 1);
		checkType(aState, -1, LUA_TNUMBER, aFunc);
		aArray[i] = (float)lua_tonumber(aState, -1);
		lua_pop(aState, 1);
	}
	return n;
}

int CParticleSystemLoader::getColorArray(lua_State *aState, int aIndex, const char *aFunc, D3DCOLOR *&aArray)
{
	checkType(aState, aIndex, LUA_TTABLE, aFunc);
	int n = luaL_getn(aState, aIndex); // get size of table
	aArray = new D3DCOLOR[n];
	for (int i = 0; i < n; i++)
	{
		lua_rawgeti(aState, aIndex, i + 1);
		checkType(aState, -1, LUA_TNUMBER, aFunc);
		aArray[i] = (D3DCOLOR)lua_tonumber(aState, -1);
		lua_pop(aState, 1);
	}
	return n;
}

int CParticleSystemLoader::getNumberFromTable(lua_State *aState, int aLuaType, const char *aKey, const char *aFunc)
{
	char temp[128];
	lua_pushstring(aState, aKey);
	lua_rawget(aState, -2);
	sprintf(temp, "%s, %s", aFunc, aKey);
	checkType(aState, -1, aLuaType, temp);
	int value;
	if (aLuaType == LUA_TNUMBER)
		value = (int)lua_tonumber(aState, -1);
	else if (aLuaType == LUA_TBOOLEAN)
		value = (int)lua_toboolean(aState, -1);
	lua_pop(aState, 1);
	return value;
}

int CParticleSystemLoader::initParticleSystem(lua_State *aState)
{
	checkType(aState, -1, LUA_TTABLE, "initParticleSystem");

	int particles = getNumberFromTable(aState, LUA_TNUMBER, "Particles", "initParticleSystem");
	mPosNum = getNumberFromTable(aState, LUA_TNUMBER, "Coordinates", "initParticleSystem");
	mColorNum = getNumberFromTable(aState, LUA_TNUMBER, "Colors", "initParticleSystem");
	mSizeNum = getNumberFromTable(aState, LUA_TNUMBER, "Sizes", "initParticleSystem");
	int duration = getNumberFromTable(aState, LUA_TNUMBER, "Duration", "initParticleSystem");
	bool looping = getNumberFromTable(aState, LUA_TBOOLEAN, "Looping", "initParticleSystem") != 0;
	lua_pushstring(aState, "Texture");
	lua_rawget(aState, -2);
	checkType(aState, -1, LUA_TSTRING, "initParticleSystem, Textures");
	const char *texture = lua_tostring(aState, -1);
	// init
	int last = 0;
	for (int i = 0; mFileName[i]; i++)
		if (mFileName[i] == '/')
			last = i;
	char temp[256];
	strncpy(temp, mFileName, last + 1);
	temp[last + 1] = '\0';
	strcat(temp, texture);
	mParticleSystem = new CParticleSystem(particles, duration, mPosNum, mColorNum, mSizeNum, temp);
	mParticleSystem->enableLooping(looping);
	lua_pop(aState, 2);
	return 0;
}

int CParticleSystemLoader::setParticle(lua_State *aState)
{
	for (int i = 1; i <= 3; i++)
		checkType(aState, i, LUA_TNUMBER, "setParticle");
	int index = (int)lua_tonumber(aState, 1) - 1;
	int life = (int)lua_tonumber(aState, 2);
	int startingTime = (int)lua_tonumber(aState, 3);
	D3DXVECTOR3 *pos = NULL;
	int posNum = getPosArray(aState, 4, "setParticle", pos);
	D3DCOLOR *color = NULL;
	int colorNum = 0;
	if (lua_gettop(aState) >= 5)
		colorNum = getColorArray(aState, 5, "setParticle", color);
	float *size = NULL;
	int sizeNum = 0;
	if (lua_gettop(aState) >= 6)
		sizeNum = getFloatArray(aState, 6, "setParticle", size);
	lua_pop(aState, size ? 6 : color ? 5 : 4);
	if (posNum != mPosNum || (color && colorNum != mColorNum) || (size && sizeNum != mSizeNum))
	{
		lua_pushstring(aState, "setParticle: Invalid array length");
		lua_error(aState);
	}
	// TODO: colors, sizes, check for size of each array
	mParticleSystem->setParticle(index, life, startingTime, pos, color, size);
	return 0;
}

int CParticleSystemLoader::rgba(lua_State *aState)
{
	for (int i = -4; i < 0; i++)
		checkType(aState, i, LUA_TNUMBER, "rgba");
	unsigned char r = (unsigned char)lua_tonumber(aState, -4);
	unsigned char g = (unsigned char)lua_tonumber(aState, -3);
	unsigned char b = (unsigned char)lua_tonumber(aState, -2);
	unsigned char a = (unsigned char)lua_tonumber(aState, -1);
	lua_pop(aState, 4);
	lua_pushnumber(aState, (a << 24) | (r << 16) | (g << 8) | b);
	return 1;
}

int CParticleSystemLoader::rgb(lua_State *aState)
{
	for (int i = -3; i < 0; i++)
		checkType(aState, i, LUA_TNUMBER, "rgb");
	unsigned char r = (unsigned char)lua_tonumber(aState, -3);
	unsigned char g = (unsigned char)lua_tonumber(aState, -2);
	unsigned char b = (unsigned char)lua_tonumber(aState, -1);
	lua_pop(aState, 3);
	lua_pushnumber(aState, (0xFF << 24) | (r << 16) | (g << 8) | b);
	return 1;
}

int CParticleSystemLoader::setDefaultColor(lua_State *aState)
{
	D3DCOLOR *color = NULL;
	int num = getColorArray(aState, -1, "setDefaultColor", color);
	mParticleSystem->setDefaultColor(color);
	if (num != mColorNum)
	{
		lua_pushstring(aState, "setDefaultColor: Invalid array length");
		lua_error(aState);
	}
	lua_pop(aState, 1);
	return 0;
}

int CParticleSystemLoader::setDefaultSize(lua_State *aState)
{
	float *size = NULL;
	int num = getFloatArray(aState, -1, "setDefaultSize", size);
	mParticleSystem->setDefaultSize(size);
	if (num != mSizeNum)
	{
		lua_pushstring(aState, "setDefaultSize: Invalid array length");
		lua_error(aState);
	}
	lua_pop(aState, 1);
	return 0;
}
