// This is the main DLL file.

#include "stdafx.h"

#include "ParticleLib.h"

namespace ParticleLib
{
	ParticleSystem::ParticleSystem(String *aLuaFile)
	{
		char *temp = new char[aLuaFile->Length + 1];
		for (int i = 0; i < aLuaFile->Length; i++)
			temp[i] = (char)aLuaFile->Chars[i];
		temp[i] = '\0';
		mParticleSystem = CParticleSystemLoader::load(temp);
		delete [] temp;
		if (mParticleSystem == NULL)
			throw new ApplicationException(new String(CParticleSystemLoader::getErrorMsg()));
	}

	ParticleSystem::ParticleSystem(int aParticles, char aPosNum, char aColorNum, char aSizeNum, String *aTexFile)
	{
		char *temp = new char[aTexFile->Length + 1];
		for (int i = 0; i < aTexFile->Length; i++)
			temp[i] = (char)aTexFile->Chars[i];
		temp[i] = '\0';
		mParticleSystem = new CParticleSystem(aParticles, aPosNum, aColorNum, aSizeNum, temp);
		delete [] temp;
	}

	ParticleSystem::~ParticleSystem()
	{
		delete mParticleSystem;
	}

	void ParticleSystem::setDefaultColor(const D3DCOLOR *aColor)
	{
		mParticleSystem->setDefaultColor(aColor);
	}

	void ParticleSystem::setDefaultSize(const float *aSize)
	{
		mParticleSystem->setDefaultSize(aSize);
	}

	void ParticleSystem::enableLooping(bool aLooping)
	{
		mParticleSystem->enableLooping(aLooping);
	}

	void ParticleSystem::setTexture(String *aTexFile)
	{
		char *temp = new char[aTexFile->Length + 1];
		for (int i = 0; i < aTexFile->Length; i++)
			temp[i] = (char)aTexFile->Chars[i];
		temp[i] = '\0';
		mParticleSystem->setTexture(temp);
		delete [] temp;
	}

	void ParticleSystem::setParticle(int aIndex, int aLife, const D3DXVECTOR3 *aPos, const int *aColor, const float *aSize)
	{
		mParticleSystem->setParticle(aIndex, aLife, aPos, (const D3DCOLOR *)aColor, aSize);
	}

	void ParticleSystem::draw(int aTime)
	{
		mParticleSystem->draw(aTime);
	}

	// D3DObj

	D3DObj::D3DObj(int aHWnd)
	{
		mD3DObj = new CD3DObj((HWND)aHWnd);
	}

	D3DObj::~D3DObj()
	{
		delete mD3DObj;
	}

	bool D3DObj::initWindowed()
	{
		bool res = mD3DObj->initWindowed();
		D3DXMATRIX projectionMatrix;
		ZeroMemory(&projectionMatrix, sizeof(projectionMatrix));
		float screenAspect = (float)mD3DObj->width() / (float)mD3DObj->height();
		float FOV = D3DX_PI / 4;
		D3DXMatrixPerspectiveFovLH(&projectionMatrix, FOV, screenAspect, 1.0f, 150.0f);
		mD3DObj->mD3DDevice->SetTransform(D3DTS_PROJECTION, &projectionMatrix);
		return res;
	}

	void D3DObj::beginScene()
	{
		mD3DObj->mD3DDevice->Clear(0,0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0,0,0), 1.0f, 0);
		mD3DObj->mD3DDevice->BeginScene();
	}

	void D3DObj::endScene()
	{
		mD3DObj->mD3DDevice->EndScene();
		mD3DObj->flip();
	}

	void D3DObj::setTransform(float aX, float aY, float aDist)
	{
		D3DXMATRIX viewMatrix, temp;
		D3DXMatrixRotationY(&viewMatrix, aY);
		D3DXMatrixRotationX(&temp, aX);
		D3DXMatrixMultiply(&viewMatrix, &viewMatrix, &temp);
		D3DXMatrixTranslation(&temp, 0, 0, aDist);
		D3DXMatrixMultiply(&viewMatrix, &viewMatrix, &temp);
		mD3DObj->mD3DDevice->SetTransform(D3DTS_VIEW, &viewMatrix);
	}
}
