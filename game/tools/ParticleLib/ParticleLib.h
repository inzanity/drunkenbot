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
		~ParticleSystem();

		void setTexture(String *aTexFile);
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
		void setBGColor(D3DCOLOR aColor);
	private:
		CD3DObj *mD3DObj;
		D3DCOLOR mBGColor;
	};
}
