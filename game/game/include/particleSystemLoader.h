#ifndef PARTICLE_SYSTEM_LOADER_H
#define PARTICLE_SYSTEM_LOADER_H

#include "particleSystem.h"

struct lua_State;

class CParticleSystemLoader
{
public:
	static CParticleSystem *load(const char *aFileName);
	static const char *getErrorMsg();
private:
	static void checkType(lua_State *aState, int aIndex, int aType, const char *aFunc);
	static int getPosArray(lua_State *aState, int aIndex, const char *aFunc, D3DXVECTOR3 *&aArray);
	static int getFloatArray(lua_State *aState, int aIndex, const char *aFunc, float *&aArray);
	static int getColorArray(lua_State *aState, int aIndex, const char *aFunc, D3DCOLOR *&aArray);
	static int getNumberFromTable(lua_State *aState, int aLuaType, const char *aKey, const char *aFunc);
	static int initParticleSystem(lua_State *aState);
	static int setParticle(lua_State *aState);
	static int frand(lua_State *aState);
	static int rgb(lua_State *aState);
	static int setDefaultColor(lua_State *aState);
	static int setDefaultSize(lua_State *aState);
private:
	static CParticleSystem *mParticleSystem;
	static const char *mFileName;
	static char *mErrorMsg;
//	static int mCoordinates, mColors, mSizes;
};

#endif // PARTICLE_SYSTEM_LOADER_H
