#include "../include/animation.h"
#include "../include/animationStorage.h"

// CMeshAnimation

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
