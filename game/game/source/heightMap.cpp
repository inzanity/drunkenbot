#include "../include/heightMap.h"
#include "../include/animationStorage.h"

inline D3DCOLOR lerp(D3DCOLOR x, D3DCOLOR y, float s);

CHeightMap::CHeightMap(int aHMapSize, int aVMapSize) :
	mHMapSize(aHMapSize), mVMapSize(aVMapSize), mHeightMap(NULL), mHeight(1.0f),
	mMaxHeight(0xFFFFFF), mColorNum(2), mColors(NULL),
	mTexture(NULL), mMesh(NULL),
	mNumVertices(0), mNumTriangles(0)
{
	int i, j;
	mHeightMap = new int *[mVMapSize];
	for (i = 0; i < mVMapSize; i++)
	{
		mHeightMap[i] = new int[mHMapSize];
		for (j = 0; j < mHMapSize; j++)
			mHeightMap[i][j] = rand() % 5 * mMaxHeight / 5;
	}
	mColors = new D3DCOLOR[mColorNum];
	mColors[0] = D3DCOLOR_XRGB(150, 150, 150);
	mColors[1] = D3DCOLOR_XRGB(150, 150, 150);
	restore(NULL);
}

CHeightMap::CHeightMap(const char *aFileName) :
	mHMapSize(0), mVMapSize(0), mHeightMap(NULL), mHeight(6.4f),
	mMaxHeight(0xFFFFFF), mColorNum(0), mColors(NULL),
	mTexture(NULL), mMesh(NULL),
	mNumVertices(0), mNumTriangles(0)
{
	LPDIRECT3DTEXTURE9 tex;
	D3DXIMAGE_INFO info;
	D3DXCreateTextureFromFileEx(d3dObj->mD3DDevice, aFileName, 0, 0, 1, 0, D3DFMT_UNKNOWN, D3DPOOL_SYSTEMMEM, D3DX_FILTER_NONE, D3DX_FILTER_NONE, 0, &info, NULL, &tex);
	D3DLOCKED_RECT rect;
	tex->LockRect(0, &rect, NULL, D3DLOCK_READONLY);
	int i, j;
	mHMapSize = info.Width; mVMapSize = info.Height;
	mHeightMap = new int *[mVMapSize];
	for (i = 0; i < mVMapSize; i++)
	{
		mHeightMap[i] = new int[mHMapSize];
		for (j = 0; j < mHMapSize; j++)
		{
			int *ptr = (int *)((BYTE *)rect.pBits + ((mVMapSize - i - 1) * rect.Pitch)) + j;
			mHeightMap[i][j] = *ptr;
		}
	}
	tex->UnlockRect(0);
	tex->Release();
	mColorNum = 5;
	mColors = new D3DCOLOR[mColorNum];
	mColors[0] = D3DCOLOR_XRGB(64, 64, 128);
	mColors[1] = D3DCOLOR_XRGB(96, 96, 96);
	mColors[2] = D3DCOLOR_XRGB(112, 112, 112);
	mColors[3] = D3DCOLOR_XRGB(128, 128, 128);
	mColors[4] = D3DCOLOR_XRGB(255, 128, 128);
	restore(NULL);
}

CHeightMap::~CHeightMap()
{
	release();
	delete [] mHeightMap;
	delete [] mColors;
}

void CHeightMap::draw(uint32 aTime)
{

	LPDIRECT3DDEVICE9 device = d3dObj->mD3DDevice;

	device->SetTransform(D3DTS_VIEW, d3dObj->mMatrixStack->GetTop());
	device->SetTexture(0, *mTexture);
	device->SetMaterial(&mMaterial);
	mMesh->DrawSubset(0);
}

uint32 CHeightMap::getDuration()
{
	return 0;
}

void CHeightMap::release()
{
	mMesh->Release();
	if (mTexture)
		mTexture->release();
}

void CHeightMap::restore(const char *)
{
	if (mMesh)
		return;
	mTexture = CAnimationStorage::ptr()->getTexture("data/conc04.jpg");
	memset(&mMaterial, 0, sizeof(mMaterial));
	mMaterial.Ambient.a = 1.f;
	mMaterial.Ambient.r = 1.f;
	mMaterial.Ambient.g = 1.f;
	mMaterial.Ambient.b = 1.f;
	mMaterial.Diffuse.a = 1.f;
	mMaterial.Diffuse.r = 1.f;
	mMaterial.Diffuse.g = 1.f;
	mMaterial.Diffuse.b = 1.f;

	mNumVertices = mVMapSize * mHMapSize;
	mNumTriangles = (mVMapSize - 1) * (mHMapSize - 1) * 2;
	D3DXCreateMeshFVF(mNumTriangles, mNumVertices, D3DPOOL_DEFAULT, D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1, d3dObj->mD3DDevice, &mMesh);

	{
		float *ptr = NULL;
		mMesh->LockVertexBuffer(0, (void **)&ptr);
		for (int i = 0; i < mVMapSize; i++)
			for (int j = 0; j < mHMapSize; j++)
			{
				int k = (i * mHMapSize + j) * 9;
				ptr[k + 0] = (float)j;
				ptr[k + 1] = mHeightMap[i][j] / (float)mMaxHeight * mHeight;
				ptr[k + 2] = (float)i;
				float s = mHeightMap[i][j] / (float)mMaxHeight * (mColorNum - 1);
				((D3DCOLOR *)ptr)[k + 6] =
					mColorNum == 1 ? mColors[0] : lerp(mColors[(int)s], mColors[(int)s + 1], s - (int)s);
				ptr[k + 7] = i / 10.f;
				ptr[k + 8] = j / 10.f;
			}
		mMesh->UnlockVertexBuffer();
	}

	{
		short *ptr = NULL;
		mMesh->LockIndexBuffer(0, (void **)&ptr);
		for (int i = 0; i < mVMapSize - 1; i++)
			for (int j = 0; j < mHMapSize - 1; j++)
			{
				int k = i * mHMapSize + j;
				int l = (i * (mHMapSize - 1) + j) * 6;
				ptr[l + 0] = k;
				ptr[l + 1] = k + mHMapSize;
				ptr[l + 2] = k + 1;
				ptr[l + 3] = k + 1;
				ptr[l + 4] = k + mHMapSize;
				ptr[l + 5] = k + mHMapSize + 1;
			}
		mMesh->UnlockIndexBuffer();
	}
	D3DXComputeNormals(mMesh, NULL);
}

float CHeightMap::height(float aX, float aY)
{
	float sX  = aX - (int)aX;
	float sY  = aY - (int)aY;
	float val1 = sX * mHeightMap[(int)aY][(int)aX] + (1.f - sX) * mHeightMap[(int)aY][(int)aX + 1];
	float val2 = sX * mHeightMap[(int)aY + 1][(int)aX] + (1.f - sX) * mHeightMap[(int)aY + 1][(int)aX + 1];
	return (sY * val1 + (1.f - sY) * val2) / (float)mMaxHeight * mHeight;
}