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

CParticleSystem *CParticleSystemLoader::load(const char *aFileName)
{
	mFileName = aFileName;
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
	lua_register(state, "frand", frand);
	lua_register(state, "rgb", rgb);
	lua_register(state, "setDefaultColor", setDefaultColor);
	lua_register(state, "setDefaultSize", setDefaultSize);

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
	int coords = getNumberFromTable(aState, LUA_TNUMBER, "Coordinates", "initParticleSystem");
	int colors = getNumberFromTable(aState, LUA_TNUMBER, "Colors", "initParticleSystem");
	int sizes = getNumberFromTable(aState, LUA_TNUMBER, "Sizes", "initParticleSystem");
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
	mParticleSystem = new CParticleSystem(particles, coords, colors, sizes, temp);
//	std::cout<<"particles: "<<particles<<", coords: "<<coords<<", colors: "<<colors<<", sizes: "<<sizes<<", looping: "<<looping<<", texture: "<<texture<<std::endl;
	lua_pop(aState, 2);
	return 0;
}

int CParticleSystemLoader::setParticle(lua_State *aState)
{
	checkType(aState, -3, LUA_TNUMBER, "setParticle");
	int index = (int)lua_tonumber(aState, -3) - 1;
	checkType(aState, -2, LUA_TNUMBER, "setParticle");
	int life = (int)lua_tonumber(aState, -2);
	D3DXVECTOR3 *pos = NULL;
	int num = getPosArray(aState, -1, "setParticle", pos);
//	std::cout<<"found " << num << "*3 positions"<<std::endl;
	lua_pop(aState, 3);
	// TODO: colors, sizes, check for size of each array
	mParticleSystem->setParticle(index, life, pos, NULL, NULL);
	return 0;
}

int CParticleSystemLoader::frand(lua_State *aState)
{
	lua_pushnumber(aState, 2.f * rand() / (RAND_MAX + 1.f) - 1.f);
	return 1;
}

int CParticleSystemLoader::rgb(lua_State *aState)
{
	checkType(aState, -3, LUA_TNUMBER, "rgb");
	checkType(aState, -2, LUA_TNUMBER, "rgb");
	checkType(aState, -1, LUA_TNUMBER, "rgb");
	unsigned char r = (unsigned char)lua_tonumber(aState, -3);
	unsigned char g = (unsigned char)lua_tonumber(aState, -2);
	unsigned char b = (unsigned char)lua_tonumber(aState, -1);
	lua_pop(aState, 3);
	lua_pushnumber(aState, (r << 16) | (g << 8) | b);
//	std::cout<<"r: "<<(int)r<<" g: "<<(int)g<<" b: "<<(int)b<<std::endl;
	return 1;
}

int CParticleSystemLoader::setDefaultColor(lua_State *aState)
{
	D3DCOLOR *color = NULL;
	int num = getColorArray(aState, -1, "setDefaultColor", color);
//	std::cout<<"found "<<num<<" colors"<<std::endl;
	mParticleSystem->setDefaultColor(color);
	return 0;
}

int CParticleSystemLoader::setDefaultSize(lua_State *aState)
{
	float *size = NULL;
	int num = getFloatArray(aState, -1, "setDefaultSize", size);
//	std::cout<<"found "<<num<<" sizes"<<std::endl;
	mParticleSystem->setDefaultSize(size);
	return 0;
}
