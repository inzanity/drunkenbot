#include "stdafx.h"

#include "../include/particleSystem.h"

inline DWORD FtoDW(FLOAT f) {return *((DWORD*)&f);}

CParticleSystem::CParticleSystem(int aParticles, char aPosNum, char aColorNum, char aSizeNum, const char *aTexFile) :
	mFVF(D3DFVF_XYZ), mVertexSize(3 * sizeof(float)),
	mParticles(aParticles), mPosNum(aPosNum), mColorNum(aColorNum), mSizeNum(aSizeNum),
	mPos(NULL), mColor(NULL), mSize(NULL), mLife(NULL),
	mTexture(NULL), mVertexBuffer(NULL),
	mDefaultColor(false), mDefaultSize(false)

{
	mPos = new D3DXVECTOR3 *[mParticles];
	mLife = new int[mParticles];
	for (int i = 0; i < mParticles; i++)
	{
		mPos[i]= new D3DXVECTOR3[mPosNum];
		mLife[i] = 0;
	}
	HRESULT res;
	res = D3DXCreateTextureFromFile(d3dObj->mD3DDevice, aTexFile, &mTexture);
	if (FAILED(res))
		res = res; // todo
}

CParticleSystem::~CParticleSystem()
{
	if (mVertexBuffer)
		mVertexBuffer->Release();
	if (mTexture)
		mTexture->Release();
}


void CParticleSystem::setDefaultColor(const D3DCOLOR *aColor)
{
	if (/*TODO mColorNum > 1 && */(mFVF & D3DFVF_DIFFUSE) == 0)
	{
		mFVF |= D3DFVF_DIFFUSE;
		mVertexSize += sizeof(float);
	}
	mDefaultColor = true;
	if (!mColor)
	{
		mColor = new D3DCOLOR *[1];
		mColor[0] = new D3DCOLOR[mColorNum];
	}
	for (int i = 0; i < mColorNum; i++)
		mColor[0][i] = aColor[i];
}

void CParticleSystem::setDefaultSize(const float *aSize)
{
	if (mSizeNum > 1 && (mFVF & D3DFVF_PSIZE) == 0)
	{
		mFVF |= D3DFVF_PSIZE;
		mVertexSize += sizeof(float);
	}
	mDefaultSize = true;
	if (!mSize)
	{
		mSize = new float *[1];
		mSize[0] = new float[mSizeNum];
	}
	for (int i = 0; i < mSizeNum; i++)
		mSize[0][i] = aSize[i];
}

void CParticleSystem::enableLooping(bool aLooping)
{
	mLooping = aLooping;
}

void CParticleSystem::setTexture(const char *aTexFile)
{
	if (mTexture)
		mTexture->Release();
	D3DXCreateTextureFromFile(d3dObj->mD3DDevice, aTexFile, &mTexture);
}

void CParticleSystem::setParticle(int aIndex, int aLife, const D3DXVECTOR3 *aPos, const D3DCOLOR *aColor, const float *aSize)
{
	int i;
	if (!mColor && aColor && !mDefaultColor)
	{
		mFVF |= D3DFVF_DIFFUSE;
		mVertexSize += sizeof(float);
		mColor = new D3DCOLOR *[mParticles];
		for (i = 0; i < mParticles; i++)
			mColor[i] = new D3DCOLOR[mColorNum];
	}
	if (!mSize && aSize && !mDefaultSize)
	{
		mFVF |= D3DFVF_PSIZE;
		mVertexSize += sizeof(float);
		mSize = new float *[mParticles];
		for (i = 0; i < mParticles; i++)
			mSize[i] = new float[mSizeNum];
	}

	for (i = 0; i < mPosNum; i++)
		mPos[aIndex][i] = aPos[i];
	mLife[aIndex] = aLife;

	if (aColor)
		for (i = 0; i < mColorNum; i++)
			mColor[aIndex][i] = aColor[i];
	if (aSize)
		for (i = 0; i < mSizeNum; i++)
			mSize[aIndex][i] = aSize[i];
}

void CParticleSystem::draw(int aTime)
{
	LPDIRECT3DDEVICE9 device = d3dObj->mD3DDevice;

	if (!mVertexBuffer)
		d3dObj->mD3DDevice->CreateVertexBuffer(mVertexSize * mParticles,
			D3DUSAGE_POINTS | D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC,
			mFVF, D3DPOOL_DEFAULT, &mVertexBuffer, NULL);

	// Set up point sprites
    device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	device->SetRenderState(D3DRS_LIGHTING, FALSE);
	device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	device->SetRenderState(D3DRS_POINTSPRITEENABLE, TRUE);
	device->SetRenderState(D3DRS_POINTSCALEENABLE, TRUE);
	device->SetRenderState(D3DRS_POINTSCALE_A, FtoDW(0.0f));
	device->SetRenderState(D3DRS_POINTSCALE_B, FtoDW(1.0f));
	if ((mFVF & D3DFVF_PSIZE) == false && mSize)
	    device->SetRenderState(D3DRS_POINTSIZE, FtoDW(mSize[0][0]));
//    device->SetRenderState( D3DRS_POINTSIZE_MIN, FtoDW(1.0f) ); // Float value that specifies the minimum size of point primitives. Point primitives are clamped to this size during rendering. 
	if ((mFVF & D3DFVF_DIFFUSE) == false && mColor)
	{
		device->SetRenderState(D3DRS_COLORVERTEX, TRUE);
		device->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, mColor[0][0]); // TODO!
	}

	float *ptr = NULL;
	mVertexBuffer->Lock(0, mVertexSize * mParticles, (void**)&ptr, D3DLOCK_DISCARD);
	int j = 0;
	for (int i = 0; i < mParticles; i++)
	{
		if (mLife[i] <= 0) continue;
		int time = (mLooping ? aTime % mLife[i] : aTime);
		float s = mPosNum * time / (float)mLife[i];
		D3DXVec3Lerp((D3DXVECTOR3 *)&ptr[j], &mPos[i][(int)s], &mPos[i][(int)s + 1], s - (int)s);
		j += 3;
		if (mFVF & D3DFVF_PSIZE)
			ptr[j++] = mDefaultSize ? mSize[0][0] : mSize[i][0];
		if (mFVF & D3DFVF_DIFFUSE)
			((D3DCOLOR *)ptr)[j++] = mDefaultColor ? mColor[0][0] : mColor[i][0];
	}
	mVertexBuffer->Unlock();

	device->SetStreamSource(0, mVertexBuffer, 0, mVertexSize);
	device->SetFVF(mFVF);
	device->SetTexture(0, mTexture);
	// Render all our particles
	HRESULT res = device->DrawPrimitive(D3DPT_POINTLIST, 0, mParticles);
	if (FAILED(res))
		int teppo = 4 * 3;
}
