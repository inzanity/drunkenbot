#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include "animation.h"

class CParticleSystem : public MAnimation
{
private:
public:
	CParticleSystem(int aParticles, int aDuration, char aPosNum, char aColorNum, char aSizeNum, const char *aTexFile);
	~CParticleSystem();

	void setDefaultColor(const D3DCOLOR *aColor);
	void setDefaultSize(const float *aSize);
	void enableLooping(bool aLooping);
	void setTexture(const char *aTexFile);

	void setParticle(int aIndex, int aLife, int aStartingTime, const D3DXVECTOR3 *aPos, const D3DCOLOR *aColor, const float *aSize);

	void draw(uint32 aTime);
	uint32 getDuration();
	void release();
	void restore(const char *aFileName);
private:
	int				mDuration;
	int				mFVF;
	int				mVertexSize;
	int				mParticles;
	bool			mLooping;

	D3DXVECTOR3 **	mPos;
	D3DCOLOR **		mColor;
	float **		mSize;
	int *			mLife;
	int *			mStartingTime;

	bool			mDefaultColor;
	bool			mDefaultSize;

	char			mPosNum;
	char			mColorNum;
	char			mSizeNum;

	CTexture *		mTexture;
	LPDIRECT3DVERTEXBUFFER9	mVertexBuffer;
};

#endif // PARTICLE_SYSTEM_H