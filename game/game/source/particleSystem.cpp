#include "stdafx.h"

#include "../include/particleSystem.h"
#include "../include/animationStorage.h"

#define COLOR_A(x)	((signed)(x >> 24))
#define COLOR_R(x)	((signed)((x >> 16) & 0xFF))
#define COLOR_G(x)	((signed)((x >> 8) & 0xFF))
#define COLOR_B(x)	((signed)(x & 0xFF))

inline float lerp(float x, float y, float s)
{
	return (x + (y - x) * s);
}
inline D3DCOLOR lerp(D3DCOLOR x, D3DCOLOR y, float s)
{
	return D3DCOLOR_ARGB(	(D3DCOLOR)(COLOR_A(x) + (COLOR_A(y) - COLOR_A(x)) * s),
							(D3DCOLOR)(COLOR_R(x) + (COLOR_R(y) - COLOR_R(x)) * s),
							(D3DCOLOR)(COLOR_G(x) + (COLOR_G(y) - COLOR_G(x)) * s),
							(D3DCOLOR)(COLOR_B(x) + (COLOR_B(y) - COLOR_B(x)) * s));
}


CParticleSystem::CParticleSystem(int aParticles, int aDuration, char aPosNum, char aColorNum, char aSizeNum, const char *aTexFile) :
	mFVF(D3DFVF_XYZ), mVertexSize(3 * sizeof(float)), mDuration(aDuration),
	mParticles(aParticles), mPosNum(aPosNum), mColorNum(aColorNum), mSizeNum(aSizeNum),
	mPos(NULL), mColor(NULL), mSize(NULL), mLife(NULL),
	mTexture(NULL), mVertexBuffer(NULL),
	mDefaultColor(false), mDefaultSize(false)

{
	mPos = new D3DXVECTOR3 *[mParticles];
	mLife = new int[mParticles];
	mStartingTime = new int[mParticles];
	int posNum = (mPosNum <= 2 ? mPosNum : mPosNum + 2);
	for (int i = 0; i < mParticles; i++)
	{
		mPos[i]= new D3DXVECTOR3[posNum];
		for (int j = 0; j < posNum; j++)
			mPos[i][j] = D3DXVECTOR3(0, 0, 0);
		mLife[i] = 0;
		mStartingTime[i] = 0;
	}
	mTexture = CAnimationStorage::ptr()->getTexture(aTexFile);
}

CParticleSystem::~CParticleSystem()
{
	release();
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
	mTexture = CAnimationStorage::ptr()->getTexture(aTexFile);
}

void CParticleSystem::setParticle(int aIndex, int aLife, int aStartingTime, const D3DXVECTOR3 *aPos, const D3DCOLOR *aColor, const float *aSize)
{
	int i, j;
	if (!mColor && aColor && !mDefaultColor)
	{
		mFVF |= D3DFVF_DIFFUSE;
		mVertexSize += sizeof(float);
		mColor = new D3DCOLOR *[mParticles];
		for (i = 0; i < mParticles; i++)
		{
			mColor[i] = new D3DCOLOR[mColorNum];
			for (j = 0; j < mColorNum; j++)
				mColor[i][j] = RGB(255, 255, 255);
		}
	}
	if (!mSize && aSize && !mDefaultSize)
	{
		mFVF |= D3DFVF_PSIZE;
		mVertexSize += sizeof(float);
		mSize = new float *[mParticles];
		for (i = 0; i < mParticles; i++)
		{
			mSize[i] = new float[mSizeNum];
			for (j = 0; j < mSizeNum; j++)
				mSize[i][j] = .1f;
		}
	}

	if (mPosNum > 2)
	{
		for (i = 0; i < mPosNum; i++)
			mPos[aIndex][i + 1] = aPos[i];
		D3DXVec3Add(&mPos[aIndex][0], &aPos[0], D3DXVec3Subtract(&mPos[aIndex][0], &aPos[0], &aPos[1]));
		D3DXVec3Add(&mPos[aIndex][mPosNum+1], &aPos[mPosNum-1], D3DXVec3Subtract(&mPos[aIndex][mPosNum+1], &aPos[mPosNum-1], &aPos[mPosNum-2]));
	}
	else
		for (i = 0; i < mPosNum; i++)
			mPos[aIndex][i] = aPos[i];

	mLife[aIndex] = aLife;
	mStartingTime[aIndex] = aStartingTime;

	if (aColor)
		for (i = 0; i < mColorNum; i++)
			mColor[aIndex][i] = aColor[i];
	if (aSize)
		for (i = 0; i < mSizeNum; i++)
			mSize[aIndex][i] = aSize[i];
}

void CParticleSystem::draw(uint32 aTime)
{
	LPDIRECT3DDEVICE9 device = d3dObj->mD3DDevice;

	if (!mVertexBuffer)
		restore(NULL);

	// Set up point sprites
    device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	device->SetRenderState(D3DRS_LIGHTING, FALSE);
	device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	device->SetRenderState(D3DRS_POINTSPRITEENABLE, TRUE);
	if ((mFVF & D3DFVF_PSIZE) == false && mSize)
	    device->SetRenderState(D3DRS_POINTSIZE, *((DWORD*)&mSize[0][0]));
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
		int t = aTime + mStartingTime[i];
		if (mLife[i] <= 0 || (!mLooping && t >= mLife[i])) continue;
		int time = (mLooping ? t % mLife[i] : t);
		float s = (mPosNum - 1) * time / (float)mLife[i];
		int is = (int)s;
		if (mPosNum == 2)
			D3DXVec3Lerp((D3DXVECTOR3 *)&ptr[j], &mPos[i][is], &mPos[i][is + 1], s - is);
		else if (mPosNum > 2)
			D3DXVec3CatmullRom((D3DXVECTOR3 *)&ptr[j], &mPos[i][is], &mPos[i][is + 1], &mPos[i][is + 2], &mPos[i][is + 3], s - is);
		else
			memcpy(&ptr[j], mPos[i][0], 3 * sizeof(float));
		j += 3;
		if (mFVF & D3DFVF_PSIZE)
		{
			s = (mSizeNum - 1) * time / (float)mLife[i];
			is = (int)s;
			if (mDefaultSize)
				ptr[j] = lerp(mSize[0][is], mSize[0][is + 1], s - is);
			else
				ptr[j] = mSizeNum == 1 ? mSize[i][0] : lerp(mSize[i][is], mSize[i][is + 1], s - is);
			j++;
		}
		if (mFVF & D3DFVF_DIFFUSE)
		{
			s = (mColorNum - 1) * time / (float)mLife[i];
			is = (int)s;
			if (mDefaultColor) // TODO mDefaultColor && mColorNum == 1 should not be per-vertex
				((D3DCOLOR *)ptr)[j] = mColorNum == 1 ? mColor[0][0] : lerp(mColor[0][is], mColor[0][is + 1], s - is);
			else
				((D3DCOLOR *)ptr)[j] = mColorNum == 1 ? mColor[i][0] : lerp(mColor[i][is], mColor[i][is + 1], s - is);

			j++;
		}
	}
	mVertexBuffer->Unlock();

	device->SetStreamSource(0, mVertexBuffer, 0, mVertexSize);
	device->SetFVF(mFVF);
	device->SetTexture(0, *mTexture);
	// Render all our particles
	HRESULT res = device->DrawPrimitive(D3DPT_POINTLIST, 0, mParticles);

	// Restore render state
    device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	device->SetRenderState(D3DRS_LIGHTING, TRUE);
	device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	device->SetRenderState(D3DRS_POINTSPRITEENABLE, FALSE);
}

uint32 CParticleSystem::getDuration()
{
	return mDuration;
}

void CParticleSystem::release()
{
	if (mVertexBuffer)
	{
		mVertexBuffer->Release();
		mVertexBuffer = NULL;
	}
	if (mTexture)
		mTexture->release();
	// TODO: rest
}

void CParticleSystem::restore(const char *)
{
	if (!mVertexBuffer)
		d3dObj->mD3DDevice->CreateVertexBuffer(mVertexSize * mParticles,
			D3DUSAGE_POINTS | D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC,
			mFVF, D3DPOOL_DEFAULT, &mVertexBuffer, NULL);
}
