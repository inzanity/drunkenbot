#include "../include/particleSystemLoader.h"
#include "../include/animationStorage.h"

CAnimationStorage *CAnimationStorage::mSingletonPtr = NULL;

CAnimationStorage *CAnimationStorage::ptr()
{
	return mSingletonPtr;
}

CAnimationStorage::CAnimationStorage()
{
	mSingletonPtr = this;
}

CAnimationStorage::~CAnimationStorage()
{
	releaseAll();
	mSingletonPtr = NULL;
}

MAnimation *CAnimationStorage::getAnimation(const char *aFileName)
{
	map<string, MAnimation *>::iterator i = mAnimations.find(aFileName);
	if (i == mAnimations.end())
	{
		MAnimation *anim;
		size_t l = strlen(aFileName);
		if (l > 4 && !stricmp(&aFileName[l - 4], ".lua"))
			anim = CParticleSystemLoader::load(aFileName);
		else if (l > 2 && !stricmp(&aFileName[l - 2], ".x"))
			anim = new CMeshAnimation(aFileName);
		else
			return NULL;
		mAnimations[aFileName] = anim;
		return anim;
	}
	else
		return (*i).second;
}

CTexture *CAnimationStorage::getTexture(const char *aFileName)
{
	map<string, CTexture *>::iterator i = mTextures.find(string(aFileName));
	if (i == mTextures.end())
	{
		CTexture *texture = new CTexture(aFileName);
		if (*texture)
		{
			mTextures[aFileName] = texture;
			return texture;
		}
		else
			return NULL;
	}
	else
		return (*i).second;
}

void CAnimationStorage::releaseAll()
{
	map<string, CTexture *>::iterator i;
	for (i = mTextures.begin(); i != mTextures.end(); ++i)
		(*i).second->release();

	map<string, MAnimation *>::iterator j;
	for (j = mAnimations.begin(); j != mAnimations.end(); ++j)
		(*j).second->release();
}

void CAnimationStorage::restoreAll()
{
	map<string, CTexture *>::iterator i;
	for (i = mTextures.begin(); i != mTextures.end(); ++i)
		(*i).second->restore((*i).first.c_str());

	map<string, MAnimation *>::iterator j;
	for (j = mAnimations.begin(); j != mAnimations.end(); ++j)
		(*j).second->restore((*j).first.c_str());
}
