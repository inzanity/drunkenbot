#ifndef ANIMATION_STORAGE_H
#define ANIMATION_STORAGE_H

#include "animation.h"

#include <map>
#include <string>

using namespace std;

class CAnimationStorage
{
public:
	static CAnimationStorage *ptr();

	CAnimationStorage();
	~CAnimationStorage();
	MAnimation *getAnimation(const char *aFileName);
	CTexture *getTexture(const char *aFileName);
	void releaseAll();
	void restoreAll();
private:
	static CAnimationStorage *mSingletonPtr;
	map<string, CTexture *> mTextures;
	map<string, MAnimation *> mAnimations;
};

#endif // ANIMATION_STORAGE_H
