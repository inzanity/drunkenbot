#include "../include/animation.h"
#include "../include/animationStorage.h"

// CMeshAnimation

CMeshAnimation::CMeshAnimation(const char *aFileName) :
	mFrameRoot(NULL), mAnimController(NULL), mCurrentTrack(0), mPrevTime(0),
	mBoneMatrices(NULL), mMaxBones(0), mUpdateBoundingData(true), mRadiusSqr(0)
{
	mBoundingBox.mMin = D3DXVECTOR3(FLT_MAX, FLT_MAX, FLT_MAX);
	mBoundingBox.mMax = D3DXVECTOR3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	restore(aFileName);
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
			mAnimController->SetTrackPosition(0, 0);
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
	if (mBoneMatrices)
	{
		delete [] mBoneMatrices;
		mBoneMatrices = NULL;
	}
}

void CMeshAnimation::restore(const char *aFileName)
{
	loadXFile(aFileName);
	d3dObj->mMatrixStack->LoadIdentity();
	draw(0);
	mUpdateBoundingData = false;
}

uint32 CMeshAnimation::getDuration() const
{
	LPD3DXANIMATIONSET set;
	if (!mAnimController)
		return 0;
	mAnimController->GetTrackAnimationSet(0, &set);
	double period = set->GetPeriod();
	return (uint32)(period * 1000.f);
}

const TBox *CMeshAnimation::getBoundingBox() const
{
	return &mBoundingBox;
}

float CMeshAnimation::getRadiusSqr() const
{
	return mRadiusSqr;
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
	CFrame *frame = (CFrame*)aFrameBase;
	CMeshContainer *meshContainer = (CMeshContainer*)aMeshContainerBase;
	LPDIRECT3DDEVICE9 device = d3dObj->mD3DDevice;
	UINT i;

	if (meshContainer->pSkinInfo) // Perform software skinning
	{
		D3DXMATRIX id;
		D3DXMatrixIdentity(&id);
		device->SetTransform(D3DTS_VIEW, &id);
		UINT bones = meshContainer->pSkinInfo->GetNumBones();
		for (i = 0; i < bones; ++i)
			D3DXMatrixMultiply(&mBoneMatrices[i], &meshContainer->mBoneOffsets[i], meshContainer->mFrameMatrices[i]);

		// Lock the meshes' vertex buffers
		void *SrcPtr, *DestPtr;
		meshContainer->MeshData.pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&SrcPtr);
		meshContainer->mSkinnedMesh->LockVertexBuffer(0, (void**)&DestPtr);

		// Update the skinned mesh using provided transformations
		meshContainer->pSkinInfo->UpdateSkinnedMesh(mBoneMatrices, NULL, SrcPtr, DestPtr);

		// Unlock the meshes vertex buffers
		meshContainer->mSkinnedMesh->UnlockVertexBuffer();
		meshContainer->MeshData.pMesh->UnlockVertexBuffer();
		if (mUpdateBoundingData)
			updateBoundingData(meshContainer->MeshData.pMesh, NULL);
		else
			for (i = 0; i < meshContainer->mNumAttributeGroups; i++) // Draw all subsets
			{
				int id = meshContainer->mAttributeTable[i].AttribId;
				device->SetMaterial(&(meshContainer->pMaterials[id].MatD3D));
				device->SetTexture(0, meshContainer->mTextures[id] ? *meshContainer->mTextures[id] : NULL);
				meshContainer->mSkinnedMesh->DrawSubset(id);
			}
	}
	else // Basic mesh
	{
		device->SetTransform(D3DTS_VIEW, &frame->mCombinedTransformationMatrix);

		if (mUpdateBoundingData)
			updateBoundingData(meshContainer->MeshData.pMesh, &frame->mCombinedTransformationMatrix);
		else
			for (i = 0; i < meshContainer->NumMaterials; i++) // Draw all subsets
			{
				device->SetMaterial(&meshContainer->pMaterials[i].MatD3D);
				device->SetTexture(0, meshContainer->mTextures[i] ? *meshContainer->mTextures[i] : NULL);
				meshContainer->MeshData.pMesh->DrawSubset(i);
			}
	}
}

void CMeshAnimation::updateFrameMatrices(const D3DXFRAME *aFrameBase, const D3DXMATRIX *aParentMatrix)
{
    CFrame *currentFrame = (CFrame*)aFrameBase;

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
	HRESULT res = D3DXLoadMeshHierarchyFromX(aFileName, D3DXMESH_MANAGED, d3dObj->mD3DDevice,
											 &alloc, NULL, &mFrameRoot, &mAnimController);
	if(mFrameRoot)
	{
		//Set the bones up
		setupBoneMatrices((CFrame *)mFrameRoot, NULL);

		//Setup the bone matrices array 
		mBoneMatrices  = new D3DXMATRIX[mMaxBones];
		ZeroMemory(mBoneMatrices, sizeof(D3DXMATRIX)*mMaxBones);
	}

	return res;
}

void CMeshAnimation::setupBoneMatrices(CFrame *aFrame, LPD3DXMATRIX aParentMatrix)
{
	CMeshContainer *mesh = (CMeshContainer *)aFrame->pMeshContainer;

	// Set up the bones on the mesh
	if (mesh)
	{
		// if there is a skinned mesh, then setup the bone matrices
		if (mesh->pSkinInfo)
		{
			// Create a copy of the mesh
			mesh->MeshData.pMesh->CloneMeshFVF(D3DXMESH_MANAGED, 
				mesh->MeshData.pMesh->GetFVF(), d3dObj->mD3DDevice, 
				&mesh->mSkinnedMesh);

			if (mMaxBones < mesh->pSkinInfo->GetNumBones())
				mMaxBones = mesh->pSkinInfo->GetNumBones();

			CFrame *tempFrame = NULL;

			//For each bone 
			for (UINT i = 0; i < mesh->pSkinInfo->GetNumBones(); i++)
			{   
				// Find the frame
				tempFrame = (CFrame *)D3DXFrameFind(mFrameRoot,
					mesh->pSkinInfo->GetBoneName(i));
				//set the bone part
				mesh->mFrameMatrices[i] = &tempFrame->mCombinedTransformationMatrix;
			}
		}
	}

	//Check your Sister
	if (aFrame->pFrameSibling)
		setupBoneMatrices((CFrame *)aFrame->pFrameSibling, aParentMatrix);

	//Check your Son
	if (aFrame->pFrameFirstChild)
		setupBoneMatrices((CFrame *)aFrame->pFrameFirstChild,
			&aFrame->mCombinedTransformationMatrix);
}

void CMeshAnimation::updateBoundingData(LPD3DXMESH aMesh, const D3DXMATRIX *aTransform)
{
	BYTE *ptr;
	D3DXVECTOR3 min, max;
	aMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&ptr);
	DWORD vertices = aMesh->GetNumVertices();
	DWORD stride = aMesh->GetNumBytesPerVertex();
//	D3DXComputeBoundingBox((const D3DXVECTOR3 *)ptr, vertices, stride, &min, &max);
	for (int i = 0; i < vertices; i++)
	{
		const D3DXVECTOR3 *p = (const D3DXVECTOR3 *)&ptr[i*stride];
		if (aTransform)
		{
			D3DXVECTOR4 pTransformed;
			D3DXVec3Transform(&pTransformed, p, aTransform);
			p = (const D3DXVECTOR3 *)&pTransformed;
		}

		float lenSqr = D3DXVec3LengthSq(p);
		if (lenSqr > mRadiusSqr) mRadiusSqr = lenSqr;
		if (p->x < mBoundingBox.mMin.x) mBoundingBox.mMin.x = p->x;
		if (p->y < mBoundingBox.mMin.y) mBoundingBox.mMin.y = p->y;
		if (p->z < mBoundingBox.mMin.z) mBoundingBox.mMin.z = p->z;
		if (p->x > mBoundingBox.mMax.x) mBoundingBox.mMax.x = p->x;
		if (p->y > mBoundingBox.mMax.y) mBoundingBox.mMax.y = p->y;
		if (p->z > mBoundingBox.mMax.z) mBoundingBox.mMax.z = p->z;
	}
	aMesh->UnlockVertexBuffer();
}