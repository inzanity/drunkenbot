/** @file animation.h
 * Definition of MAnimation interface and classes for basic animations.
 *
 * @author Japa
 * @version 0.9
 */

#ifndef	ANIMATION_H
#define	ANIMATION_H

#include "meshHierarchy.h"

/**
 * General Animation interface.
 */
class MAnimation
{
public:
	virtual ~MAnimation() {}
	virtual	void draw(uint32 aTime) = 0;
	virtual uint32 getDuration() const {return 0;}
	virtual const TBox *getBoundingBox() const {return NULL;}
	virtual float getRadiusSqr() const {return 0.f;}
	virtual void release() = 0;
	virtual void restore(const char *aFileName) = 0;
};

class CMeshAnimation : public MAnimation
{
public:
	CMeshAnimation(const char *aFileName);
	virtual ~CMeshAnimation();
	void draw(uint32 aTime);
	uint32 getDuration() const;
	const TBox *getBoundingBox() const;
	float getRadiusSqr() const;
	void release();
	void restore(const char *aFileName);
private:
	void drawFrame(LPD3DXFRAME aFrame);
	void drawMeshContainer(LPD3DXMESHCONTAINER aMeshContainerBase, LPD3DXFRAME aFrameBase);
	void updateFrameMatrices(const D3DXFRAME *aFrameBase, const D3DXMATRIX *aParentMatrix);
	HRESULT loadXFile(const char *aFileName);
	void setupBoneMatrices(CFrame *aFrame, LPD3DXMATRIX aParentMatrix);
	void updateBoundingData(LPD3DXMESH aMesh, const D3DXMATRIX *aTransform);
private:
	LPD3DXFRAME mFrameRoot;
	LPD3DXANIMATIONCONTROLLER mAnimController;
	DWORD mCurrentTrack;
	uint32 mPrevTime;

	// For skinning
	D3DXMATRIX *mBoneMatrices;
	uint32 mMaxBones;

	bool mUpdateBoundingData;
	TBox mBoundingBox;
	float mRadiusSqr;
};

/*
class CMeshAnimation : public MAnimation
{
public:
					CMeshAnimation		(CMeshAnimation *aAnim);
					CMeshAnimation		(const char *aFileName);
	virtual			~CMeshAnimation		();
	void			draw				(uint32 aTime);
	uint32			getDuration			();
	void			release				();
	void			restore				(const char *aFileName);
	void			setTexture			(CTexture *aTexture);
	void			setMaterial			(D3DMATERIAL9 *aMaterial);
private:
	void			readX				(const char *aFileName);
private:
	LPD3DXMESH		mMesh;
	D3DMATERIAL9 *	mMaterial;
	CTexture *		mTexture;
};
*/
#endif // ANIMATION_H