#ifndef MESH_HIERARCHY_H
#define MESH_HIERARCHY_H

#include "texture.h"

/**
 * Struct for bounding boxes.
 */
struct TBox{D3DXVECTOR3 mMin, mMax;};

class CMeshHierarchy : public ID3DXAllocateHierarchy
{
public:
	CMeshHierarchy(const char *aPath);
	// Format of these interfaces is strictly required by D3DXLoadMeshHierarchyFromX

	// Callback to create a D3DXFRAME-derived object and initialize it
	STDMETHOD(CreateFrame)(THIS_ LPCSTR Name, LPD3DXFRAME *ppNewFrame);
	// Callback to create a D3DXMESHCONTAINER-derived object and initialize it
	STDMETHOD(CreateMeshContainer)(THIS_ LPCSTR Name, CONST D3DXMESHDATA * pMeshData,
		CONST D3DXMATERIAL * pMaterials, CONST D3DXEFFECTINSTANCE * pEffectInstances,
		DWORD NumMaterials, CONST DWORD * pAdjacency, LPD3DXSKININFO pSkinInfo, 
		LPD3DXMESHCONTAINER * ppNewMeshContainer);
	// Callback to release a D3DXFRAME-derived object
	STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME pFrameToFree);
	// Callback to release a D3DXMESHCONTAINER-derived object
	STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER pMeshContainerToFree);
private:
	const char *mPath;
};

struct CMeshContainer : public D3DXMESHCONTAINER
{
	CTexture **mTextures;

	//Skinned mesh variables
	LPD3DXMESH           mSkinnedMesh;		// The skinned mesh
    LPD3DXATTRIBUTERANGE mAttributeTable;
    DWORD                mNumAttributeGroups; 
	LPD3DXMATRIX		 mBoneOffsets;		// The bone matrix offsets
	LPD3DXMATRIX*		 mFrameMatrices;	// Pointer to the frame matrix
};


struct CFrame : public D3DXFRAME
{
	D3DXMATRIXA16 mCombinedTransformationMatrix;
};

#endif // MESH_HIERARCHY_H
