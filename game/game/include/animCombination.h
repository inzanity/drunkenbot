#ifndef ANIM_COMBINATION_H
#define ANIM_COMBINATION_H

#include "animation.h"

class CAnimCombination : public MAnimation
{
public:
	CAnimCombination(const char *aTxtFile);
	~CAnimCombination();

	void draw(uint32 aTime);
	uint32 getDuration() const;
	const TBox *getBoundingBox() const;
	float getRadiusSqr() const;
	void release();
	void restore(const char *aFileName);
private:
	int mNumAnimations;
	MAnimation **mAnimations;
	uint32 *mStartTimes;
	uint32 *mEndTimes;
	D3DXMATRIX **mMatrices;
	uint32 mDuration;
	TBox mBoundingBox;
	float mRadiusSqr;
};

#endif // ANIM_COMBINATION_H