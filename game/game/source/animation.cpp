#include "../include/animation.h"
#include "../include/animationStorage.h"

// CMeshAnimation

CMeshAnimation::CMeshAnimation(const char *aFileName) :
	mFrameRoot(NULL), mAnimController(NULL), mCurrentTrack(0), mPrevTime(0)
{
	loadXFile(aFileName);
}

CMeshAnimation::~CMeshAnimation()
{
	release();
}

void CMeshAnimation::draw(uint32 aTime)
{
	if (mAnimController && aTime != mPrevTime)
	{
		if (aTime < mPrevTime)
		{
			mAnimController->ResetTime();
			mPrevTime = 0;
		}
		mAnimController->AdvanceTime((aTime - mPrevTime) / 1000.0, NULL);
		// Update the model matrices
		mPrevTime = aTime;
	}
	updateFrameMatrices(mFrameRoot, d3dObj->mMatrixStack->GetTop());
	drawFrame(mFrameRoot);
}

void CMeshAnimation::release()
{
	if (mFrameRoot)
	{
		CMeshHierarchy alloc(NULL);
		D3DXFrameDestroy(mFrameRoot, &alloc);
		mFrameRoot=NULL;
	}
	if (mAnimController)
	{
		mAnimController->Release();
		mAnimController = NULL;
	}
}

void CMeshAnimation::restore(const char *aFileName)
{
	loadXFile(aFileName);
}

uint32 CMeshAnimation::getDuration()
{
	return 0;
}

void CMeshAnimation::drawFrame(LPD3DXFRAME aFrame)
{
	LPD3DXMESHCONTAINER meshContainer;

	meshContainer = aFrame->pMeshContainer;
	while (meshContainer != NULL)
	{
		drawMeshContainer(meshContainer, aFrame);
		meshContainer = meshContainer->pNextMeshContainer;
	}

	if (aFrame->pFrameSibling != NULL)
		drawFrame(aFrame->pFrameSibling);

	if (aFrame->pFrameFirstChild != NULL)
		drawFrame(aFrame->pFrameFirstChild);
}

void CMeshAnimation::drawMeshContainer(LPD3DXMESHCONTAINER aMeshContainerBase, LPD3DXFRAME aFrameBase)
{
	D3DXFRAME_DERIVED *frame = (D3DXFRAME_DERIVED*)aFrameBase;
	D3DXMESHCONTAINER_DERIVED *meshContainer = (D3DXMESHCONTAINER_DERIVED*)aMeshContainerBase;
	UINT i;

	d3dObj->mD3DDevice->SetTransform(D3DTS_VIEW, &frame->mCombinedTransformationMatrix);

	for (i = 0; i < meshContainer->NumMaterials; i++)
	{
		d3dObj->mD3DDevice->SetMaterial(&meshContainer->pMaterials[i].MatD3D);
		if (meshContainer->mTextures[i])
			d3dObj->mD3DDevice->SetTexture(0, *meshContainer->mTextures[i]);
		else
			d3dObj->mD3DDevice->SetTexture(0, NULL);
		meshContainer->MeshData.pMesh->DrawSubset(i);
	}
}

void CMeshAnimation::updateFrameMatrices(const D3DXFRAME *aFrameBase, const D3DXMATRIX *aParentMatrix)
{
    D3DXFRAME_DERIVED *currentFrame = (D3DXFRAME_DERIVED*)aFrameBase;

	// If parent matrix exists multiply our frame matrix by it
	if (aParentMatrix != NULL)
		D3DXMatrixMultiply(&currentFrame->mCombinedTransformationMatrix, &currentFrame->TransformationMatrix, aParentMatrix);
	else
		currentFrame->mCombinedTransformationMatrix = currentFrame->TransformationMatrix;

	// If we have a sibling recurse 
	if (currentFrame->pFrameSibling != NULL)
		updateFrameMatrices(currentFrame->pFrameSibling, aParentMatrix);

	// If we have a child recurse 
	if (currentFrame->pFrameFirstChild != NULL)
		updateFrameMatrices(currentFrame->pFrameFirstChild, &currentFrame->mCombinedTransformationMatrix);
}

HRESULT CMeshAnimation::loadXFile(const char *aFileName)
{
	int last = 0;
	for (int i = 0; aFileName[i]; i++)
		if (aFileName[i] == '/')
			last = i;
	char temp[256];
	strncpy(temp, aFileName, last + 1);
	temp[last + 1] = '\0';
	CMeshHierarchy alloc(temp);
	return D3DXLoadMeshHierarchyFromX(aFileName, D3DXMESH_MANAGED, d3dObj->mD3DDevice,
				&alloc, NULL, &mFrameRoot, &mAnimController);
}


// CMeshAnimation
/*
CMeshAnimation::CMeshAnimation(CMeshAnimation *aAnim) : mMesh(NULL), mMaterial(NULL), mTexture(NULL)
{
	mMesh = aAnim->mMesh;
	mMaterial = aAnim->mMaterial;
	mTexture = aAnim->mTexture;
}

CMeshAnimation::CMeshAnimation(const char *aFileName) : mMesh(NULL), mMaterial(NULL), mTexture(NULL)
{
	readX(aFileName);
}

CMeshAnimation::~CMeshAnimation()
{
	release();
}

void CMeshAnimation::draw(uint32 aTime)
{
	if (mMaterial)
		d3dObj->mD3DDevice->SetMaterial(mMaterial);
	d3dObj->mD3DDevice->SetTexture(0, mTexture ? *mTexture : NULL);
	mMesh->DrawSubset(0);
}

void CMeshAnimation::release()
{
	if (mMesh)
	{
		mMesh->Release();
		mMesh = NULL;
	}
}

void CMeshAnimation::restore(const char *aFileName)
{
}

void CMeshAnimation::setTexture(CTexture *aTexture)
{
	mTexture = aTexture;
}

void CMeshAnimation::setMaterial(D3DMATERIAL9 *aMaterial)
{
	mMaterial = aMaterial;
}

uint32 CMeshAnimation::getDuration()
{
	return 0;
}

void CMeshAnimation::readX(const char *aFileName)
{
	LPD3DXBUFFER pD3DXMtrlBuffer;

	// Load	the	mesh from the specified	file
	DWORD materialNum;
	if(FAILED(D3DXLoadMeshFromX(aFileName, 0, d3dObj->mD3DDevice, NULL,
								&pD3DXMtrlBuffer, NULL, &materialNum, &mMesh)))
		return;

	// We need to extract the material properties and texture names	from the 
	// pD3DXMtrlBuffer
	D3DXMATERIAL *d3dxMaterials	= (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();

	// Copy	the	material
	mMaterial = new D3DMATERIAL9;
	*mMaterial = d3dxMaterials[0].MatD3D;

	// Set the ambient color for the material (D3DX	does not do	this)
	mMaterial->Ambient = mMaterial->Diffuse;

	// Create the texture
	if (d3dxMaterials[0].pTextureFilename)
	{
		int last = 0;
		for (int i = 0; aFileName[i]; i++)
			if (aFileName[i] == '/')
				last = i;
		char temp[256];
		strncpy(temp, aFileName, last + 1);
		temp[last + 1] = '\0';
		strcat(temp, d3dxMaterials[0].pTextureFilename);
		mTexture = CAnimationStorage::ptr()->getTexture(temp);
	}

	// Done	with the material buffer
	pD3DXMtrlBuffer->Release();
}
*/