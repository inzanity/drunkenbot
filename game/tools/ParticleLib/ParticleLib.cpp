// This is the main DLL file.

#include "stdafx.h"

#include "ParticleLib.h"

namespace ParticleLib
{
	ParticleSystem::ParticleSystem(String *aLuaFile) : mParticleSystem(NULL)
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

	ParticleSystem::~ParticleSystem()
	{
		delete mParticleSystem;
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

	void ParticleSystem::draw(int aTime)
	{
		mParticleSystem->draw(aTime);
	}

	// D3DObj

	D3DObj::D3DObj(int aHWnd) : mD3DObj(NULL), mBGColor(0xFF000000)
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
		LPDIRECT3DDEVICE9 device = mD3DObj->mD3DDevice;
		D3DXMATRIX projectionMatrix;
		ZeroMemory(&projectionMatrix, sizeof(projectionMatrix));
		float screenAspect = (float)mD3DObj->width() / (float)mD3DObj->height();
		float FOV = D3DX_PI / 4;
		D3DXMatrixPerspectiveFovLH(&projectionMatrix, FOV, screenAspect, 1.0f, 150.0f);
		device->SetTransform(D3DTS_PROJECTION, &projectionMatrix);

		// Turn	on the zbuffer
		device->SetRenderState(D3DRS_ZENABLE, TRUE);
		// Turn	on ambient lighting	
		device->SetRenderState(D3DRS_AMBIENT, 0xffffffff);

		// General particle render states
		device->SetRenderState(D3DRS_POINTSCALEENABLE, TRUE);
		float f = 0.0f;
		device->SetRenderState(D3DRS_POINTSCALE_A, *((DWORD*)&f));
		f = 1.0f;
		device->SetRenderState(D3DRS_POINTSCALE_B, *((DWORD*)&f));
		device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
		device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		return res;
	}

	void D3DObj::beginScene()
	{
		mD3DObj->mD3DDevice->Clear(0,0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, mBGColor, 1.0f, 0);
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

	void D3DObj::setBGColor(D3DCOLOR aColor)
	{
		mBGColor = aColor;
	}
}
