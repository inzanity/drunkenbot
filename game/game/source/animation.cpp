#include "..\include\animation.h"

// CMeshAnimation

CMeshAnimation::CMeshAnimation(CMeshAnimation *aAnim) : mMesh(NULL), mMaterial(NULL), mTexture(NULL)
{
	aAnim->restore();
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
	if (mTexture)
		d3dObj->mD3DDevice->SetTexture(0, mTexture);
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

void CMeshAnimation::restore()
{
/*
	if (!mMesh)
	{
		uint8 len = strlen(mFileName);
		if (!stricmp((char	*)&mFileName[len-4], ".txt"))
			readTxt();
		else if	(!stricmp((char *)&mFileName[len-2], ".x"))
			readX();
	}
	for (uint8 i = 0; i < mMaterialNum; i++)
		mTexture[i]->restore();
	mStored = 1;
*/
}

void CMeshAnimation::setTexture(LPDIRECT3DTEXTURE9 aTexture)
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
	D3DXCreateTextureFromFile(d3dObj->mD3DDevice, d3dxMaterials[0].pTextureFilename, &mTexture);

	// Done	with the material buffer
	pD3DXMtrlBuffer->Release();
}
