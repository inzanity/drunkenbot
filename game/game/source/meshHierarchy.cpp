#include <stdio.h>

#include "../include/meshHierarchy.h"
#include "../include/animationStorage.h"
#include <dxerr9.h>

#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }

//-----------------------------------------------------------------------------
// Name: allocateName()
// Desc: Allocates memory for a string to hold the name of a frame or mesh
//-----------------------------------------------------------------------------
HRESULT allocateName(LPCTSTR Name, LPTSTR *pNewName)
{
	UINT cbLength;

	if (Name != NULL)
	{
		cbLength = lstrlen(Name) + 1;
		*pNewName = new TCHAR[cbLength];
		if (*pNewName == NULL)
			return E_OUTOFMEMORY;
		memcpy(*pNewName, Name, cbLength*sizeof(TCHAR));
	}
	else
		*pNewName = NULL;

	return S_OK;
}

/**************************************************************************************************
**************************************************************************************************/

CMeshHierarchy::CMeshHierarchy(const char *aPath) : mPath(aPath)
{
}

//-----------------------------------------------------------------------------
// Name: CMeshHierarchy::CreateFrame()
// Desc: 
//-----------------------------------------------------------------------------
HRESULT CMeshHierarchy::CreateFrame(LPCTSTR Name, LPD3DXFRAME *ppNewFrame)
{
	CFrame *pFrame;

	*ppNewFrame = NULL;

	pFrame = new CFrame;
	if (pFrame == NULL)
		return E_OUTOFMEMORY;

	HRESULT hr = allocateName(Name, &pFrame->Name);
	if (FAILED(hr))
	{
		delete pFrame;
		return E_OUTOFMEMORY;
	}

	// initialize other data members of the frame
	D3DXMatrixIdentity(&pFrame->TransformationMatrix);
	D3DXMatrixIdentity(&pFrame->mCombinedTransformationMatrix);

	pFrame->pMeshContainer = NULL;
	pFrame->pFrameSibling = NULL;
	pFrame->pFrameFirstChild = NULL;

	*ppNewFrame = pFrame;
	pFrame = NULL;
	return S_OK;
}


//-----------------------------------------------------------------------------
// Name: CMeshHierarchy::CreateMeshContainer()
// Desc: 
//-----------------------------------------------------------------------------
HRESULT CMeshHierarchy::CreateMeshContainer(THIS_
	LPCSTR Name,
	CONST D3DXMESHDATA *pMeshData,
	CONST D3DXMATERIAL *pMaterials,
	CONST D3DXEFFECTINSTANCE *,						// Not used
	DWORD NumMaterials,
	CONST DWORD *pAdjacency,
	LPD3DXSKININFO pSkinInfo,
	LPD3DXMESHCONTAINER* ppNewMeshContainer)

{
	HRESULT hr;
	CMeshContainer *pMeshContainer = NULL;
	UINT NumFaces;
	UINT i;

	LPD3DXMESH pMesh = NULL;

	*ppNewMeshContainer = NULL;

	// this sample does not handle patch meshes, so fail when one is found
	if (pMeshData->Type != D3DXMESHTYPE_MESH)
		return E_FAIL;

	// get the pMesh interface pointer out of the mesh data structure
	pMesh = pMeshData->pMesh;

	// this sample does not FVF compatible meshes, so fail when one is found
	if (pMesh->GetFVF() == 0)
		return E_FAIL;

	// allocate the overloaded structure to return as a D3DXMESHCONTAINER
	pMeshContainer = new CMeshContainer;
	if (pMeshContainer == NULL)
		return E_OUTOFMEMORY;
	memset(pMeshContainer, 0, sizeof(CMeshContainer));

	// make sure and copy the name.  All memory as input belongs to caller, interfaces can be addref'd though
	hr = allocateName(Name, &pMeshContainer->Name);
	if (FAILED(hr))
		goto e_Exit;        

	NumFaces = pMesh->GetNumFaces();

	// if no normals are in the mesh, add them
	if (!(pMesh->GetFVF() & D3DFVF_NORMAL))
	{
		pMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;

		// clone the mesh to make room for the normals
		hr = pMesh->CloneMeshFVF(pMesh->GetOptions(), 
									pMesh->GetFVF() | D3DFVF_NORMAL, 
									d3dObj->mD3DDevice, &pMeshContainer->MeshData.pMesh);
		if (FAILED(hr))
			goto e_Exit;

		// get the new pMesh pointer back out of the mesh container to use
		// NOTE: we do not release pMesh because we do not have a reference to it yet
		pMesh = pMeshContainer->MeshData.pMesh;

		// now generate the normals for the pmesh
		D3DXComputeNormals(pMesh, NULL);
	}
	else  // if no normals, just add a reference to the mesh for the mesh container
	{
		pMeshContainer->MeshData.pMesh = pMesh;
		pMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;

		pMesh->AddRef();
	}
	    
	// allocate memory to contain the material information.  This sample uses
	//   the D3D9 materials and texture names instead of the EffectInstance style materials
	pMeshContainer->NumMaterials = max(1, NumMaterials);
	pMeshContainer->pMaterials = new D3DXMATERIAL[pMeshContainer->NumMaterials];
	pMeshContainer->mTextures = new CTexture *[pMeshContainer->NumMaterials];
	pMeshContainer->pAdjacency = new DWORD[NumFaces*3];
	if (pMeshContainer->pAdjacency == NULL ||
		pMeshContainer->pMaterials == NULL ||
		pMeshContainer->mTextures == NULL)
	{
		hr = E_OUTOFMEMORY;
		goto e_Exit;
	}

	memcpy(pMeshContainer->pAdjacency, pAdjacency, sizeof(DWORD) * NumFaces*3);
	memset(pMeshContainer->mTextures, 0, sizeof(CTexture *) * pMeshContainer->NumMaterials);

	// if materials provided, copy them
	if (NumMaterials > 0)
	{
		memcpy(pMeshContainer->pMaterials, pMaterials, sizeof(D3DXMATERIAL) * NumMaterials);

		for (i = 0; i < NumMaterials; i++)
		{
			pMeshContainer->pMaterials[i].MatD3D.Ambient = pMeshContainer->pMaterials[i].MatD3D.Diffuse;
			if (pMeshContainer->pMaterials[i].pTextureFilename != NULL)
			{
				char temp[256];
				strcpy(temp, mPath);
				strcat(temp, pMeshContainer->pMaterials[i].pTextureFilename);
				pMeshContainer->mTextures[i] = CAnimationStorage::ptr()->getTexture(temp);
			}
		}
	}
	else // if no materials provided, use a default one
	{
		pMeshContainer->pMaterials[0].pTextureFilename = NULL;
		memset(&pMeshContainer->pMaterials[0].MatD3D, 0, sizeof(D3DMATERIAL9));
		pMeshContainer->pMaterials[0].MatD3D.Diffuse.r = 0.5f;
		pMeshContainer->pMaterials[0].MatD3D.Diffuse.g = 0.5f;
		pMeshContainer->pMaterials[0].MatD3D.Diffuse.b = 0.5f;
		pMeshContainer->pMaterials[0].MatD3D.Specular = pMeshContainer->pMaterials[0].MatD3D.Diffuse;
		pMeshContainer->pMaterials[0].MatD3D.Ambient = pMeshContainer->pMaterials[0].MatD3D.Diffuse;
	}
	if(pSkinInfo)
	{
		// first save off the SkinInfo and original mesh data
	    pMeshContainer->pSkinInfo = pSkinInfo;
	    pSkinInfo->AddRef();

        pMeshContainer->MeshData.pMesh->GetAttributeTable(NULL, &pMeshContainer->mNumAttributeGroups);
        pMeshContainer->mAttributeTable = new D3DXATTRIBUTERANGE[pMeshContainer->mNumAttributeGroups];
        pMeshContainer->MeshData.pMesh->GetAttributeTable(pMeshContainer->mAttributeTable, NULL);

		// Will need an array of offset matrices to move the vertices from 
		//	the figure space to the bone's space
	    UINT uBones = pSkinInfo->GetNumBones();
	    pMeshContainer->mBoneOffsets = new D3DXMATRIX[uBones];

		//Create the arrays for the bones and the frame matrices
		pMeshContainer->mFrameMatrices = new D3DXMATRIX*[uBones];

	    // get each of the bone offset matrices so that we don't need to 
		//	get them later
	    for (UINT i = 0; i < uBones; i++)
	        pMeshContainer->mBoneOffsets[i] = *(pMeshContainer->pSkinInfo->GetBoneOffsetMatrix(i));
	}
	else
	{
		pMeshContainer->pSkinInfo = NULL;
		pMeshContainer->mBoneOffsets = NULL;
		pMeshContainer->mSkinnedMesh = NULL;
		pMeshContainer->mFrameMatrices = NULL;
	}

	*ppNewMeshContainer = pMeshContainer;
	pMeshContainer = NULL;

e_Exit:

	// call Destroy function to properly clean up the memory allocated 
	if (pMeshContainer != NULL)
		DestroyMeshContainer(pMeshContainer);

	return hr;
}




//-----------------------------------------------------------------------------
// Name: CMeshHierarchy::DestroyFrame()
// Desc: 
//-----------------------------------------------------------------------------
HRESULT CMeshHierarchy::DestroyFrame(LPD3DXFRAME pFrameToFree) 
{
	SAFE_DELETE_ARRAY(pFrameToFree->Name);
	SAFE_DELETE(pFrameToFree);
	return S_OK; 
}




//-----------------------------------------------------------------------------
// Name: CMeshHierarchy::DestroyMeshContainer()
// Desc: 
//-----------------------------------------------------------------------------
HRESULT CMeshHierarchy::DestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainerBase)
{
	UINT i;
	CMeshContainer *pMeshContainer = (CMeshContainer*)pMeshContainerBase;

	SAFE_DELETE_ARRAY(pMeshContainer->Name);
	SAFE_DELETE_ARRAY(pMeshContainer->pAdjacency);
	SAFE_DELETE_ARRAY(pMeshContainer->pMaterials);

	// release all the allocated textures
	if (pMeshContainer->mTextures != NULL)
	{
		for (i = 0; i < pMeshContainer->NumMaterials; i++)
		{
//			SAFE_RELEASE(pMeshContainer->mTextures[i]);
		}
	}

	SAFE_DELETE_ARRAY(pMeshContainer->mTextures);
	SAFE_RELEASE(pMeshContainer->MeshData.pMesh);
	SAFE_RELEASE(pMeshContainer->pSkinInfo);
	SAFE_DELETE(pMeshContainer);
	return S_OK;
}
