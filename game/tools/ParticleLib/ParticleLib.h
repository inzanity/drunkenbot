// ParticleLib.h

#pragma once

#include "../../game/include/particleSystemLoader.h"
#include "../../game/include/d3dUtil.h"

using namespace System;

namespace ParticleLib
{
	public __gc class ParticleSystem
	{
	public:
		ParticleSystem(String *aLuaFile);
		ParticleSystem(int aParticles, char aPosNum, char aColorNum, char aSizeNum, String *aTexFile);
		~ParticleSystem();

		void setDefaultColor(const D3DCOLOR *aColor);
		void setDefaultSize(const float *aSize);
		void enableLooping(bool aLooping);
		void setTexture(String *aTexFile);

		void setParticle(int aIndex, int aLife, const D3DXVECTOR3 *aPos, const int *aColor, const float *aSize);

		void draw(int aTime);
	private:
		CParticleSystem *mParticleSystem;
	};

	public __gc class D3DObj
	{
	public:
		D3DObj(int aHWnd);
		~D3DObj();
		bool initWindowed();
		void beginScene();
		void endScene();
		void setTransform(float aX, float aY, float dist);
	private:
		CD3DObj *mD3DObj;
	};
}
