#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include "d3dUtil.h"

class CParticleSystem
{
private:
public:
	CParticleSystem(int aParticles, char aPosNum, char aColorNum, char aSizeNum, const char *aTexFile);
	~CParticleSystem();

	void setDefaultColor(const D3DCOLOR *aColor);
	void setDefaultSize(const float *aSize);
	void enableLooping(bool aLooping);
	void setTexture(const char *aTexFile);

	void setParticle(int aIndex, int aLife, const D3DXVECTOR3 *aPos, const D3DCOLOR *aColor, const float *aSize);

	void draw(int aTime);
private:
	int				mFVF;
	int				mVertexSize;
	int				mParticles;
	bool			mLooping;

	D3DXVECTOR3 **	mPos;
	D3DCOLOR **		mColor;
	float **		mSize;
	int *			mLife;

	bool			mDefaultColor;
	bool			mDefaultSize;

	char			mPosNum;
	char			mColorNum;
	char			mSizeNum;

	LPDIRECT3DTEXTURE9		mTexture;
	LPDIRECT3DVERTEXBUFFER9	mVertexBuffer;
};

#endif // PARTICLE_SYSTEM_H