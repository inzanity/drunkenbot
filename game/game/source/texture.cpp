#include "..\include\texture.h"

// CTexture

CTexture::CTexture(const char *aFileName) : mTexture(NULL)
{
	restore(aFileName);
}

CTexture::~CTexture()
{
	release();
}

CTexture::operator LPDIRECT3DTEXTURE9()
{
	return mTexture;
}

void CTexture::release()
{
	if (mTexture)
	{
		mTexture->Release();
		mTexture = NULL;
	}
}

void CTexture::restore(const char *aFileName)
{
	if (!mTexture)
		if(FAILED(D3DXCreateTextureFromFile(d3dObj->mD3DDevice,	aFileName, &mTexture)))
			mTexture = NULL;
}

int CTexture::width()
{
	D3DSURFACE_DESC desc;
	mTexture->GetLevelDesc(0, &desc);
	return desc.Width;
}

int CTexture::height()
{
	D3DSURFACE_DESC desc;
	mTexture->GetLevelDesc(0, &desc);
	return desc.Height;
}
