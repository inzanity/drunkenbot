#include "../include/animationStorage.h"
#include "../include/animCombination.h"
#include <fstream>

using namespace std;

CAnimCombination::CAnimCombination(const char *aTxtFile) :
	mNumAnimations(0), mAnimations(NULL), mStartTimes(NULL), mEndTimes(NULL), mMatrices(NULL), mDuration(0), mRadiusSqr(0)
{
	restore(aTxtFile);
}

CAnimCombination::~CAnimCombination()
{
	release();
}

void CAnimCombination::draw(uint32 aTime)
{
	aTime = aTime % mDuration;
	for (int i = 0; i < mNumAnimations; i++)
		if (mStartTimes[i] <= aTime && mEndTimes[i] > aTime)
		{
			if (mMatrices[i])
				d3dObj->mMatrixStack->MultMatrixLocal(mMatrices[i]);
			mAnimations[i]->draw(aTime - mStartTimes[i]);
		}
}

uint32 CAnimCombination::getDuration() const
{
	return mDuration;
}

const TBox *CAnimCombination::getBoundingBox() const
{
	return &mBoundingBox;
}

float CAnimCombination::getRadiusSqr() const
{
	return mRadiusSqr;
}

void CAnimCombination::release()
{
	if (mAnimations)
		delete [] mAnimations;
	mAnimations = NULL;
	if (mStartTimes)
		delete [] mStartTimes;
	mStartTimes = NULL;
	if (mEndTimes)
		delete [] mEndTimes;
	mEndTimes = NULL;
	if (mMatrices)
	{
		for (int i = 0; i < mNumAnimations; i++)
			if (mMatrices[i])
				delete mMatrices[i];
		delete [] mMatrices;
	}
	mMatrices = NULL;
}

void CAnimCombination::restore(const char *aFileName)
{
	int last = -1;
	for (int i = 0; aFileName[i]; i++)
		if (aFileName[i] == '/')
			last = i;
	char temp[256];
	strncpy(temp, aFileName, last + 1);
	temp[last + 1] = '\0';
	
	ifstream file(aFileName);
	file>>mNumAnimations;
	mAnimations = new MAnimation *[mNumAnimations];
	mStartTimes = new uint32[mNumAnimations];
	mEndTimes = new uint32[mNumAnimations];
	mMatrices = new D3DXMATRIX *[mNumAnimations];
	mBoundingBox.mMax = D3DXVECTOR3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	mBoundingBox.mMin = D3DXVECTOR3(FLT_MAX, FLT_MAX, FLT_MAX);
	for (int i = 0; i < mNumAnimations; i++)
	{
		file>>&temp[last + 1];
		mAnimations[i] = CAnimationStorage::ptr()->getAnimation(temp);
		file>>mStartTimes[i];
		file>>mEndTimes[i];
		file>>std::ws;
		if (file.peek() != ';' && file.peek() != EOF)
		{
			mMatrices[i] = new D3DXMATRIX;
			for (int j = 0; j < 16; j++)
				file>>(*mMatrices[i])[j];
		}
		else
			mMatrices[i] = NULL;
		file.get(); // ;
		mRadiusSqr = max(mRadiusSqr, mAnimations[i]->getRadiusSqr());
		const TBox *box = mAnimations[i]->getBoundingBox();
		mBoundingBox.mMax.x = max(mBoundingBox.mMax.x, box->mMax.x);
		mBoundingBox.mMax.y = max(mBoundingBox.mMax.y, box->mMax.y);
		mBoundingBox.mMax.z = max(mBoundingBox.mMax.z, box->mMax.z);
		mBoundingBox.mMin.x = min(mBoundingBox.mMin.x, box->mMin.x);
		mBoundingBox.mMin.y = min(mBoundingBox.mMin.y, box->mMin.y);
		mBoundingBox.mMin.z = min(mBoundingBox.mMin.z, box->mMin.z);
		mDuration = max(mDuration, mEndTimes[i]);
	}
}