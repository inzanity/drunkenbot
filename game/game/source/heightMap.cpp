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
	mHeightMap = new float *[mVMapSize];
	for (i = 0; i < mVMapSize; i++)
	{
		mHeightMap[i] = new float[mHMapSize];
		for (j = 0; j < mHMapSize; j++)
			mHeightMap[i][j] = rand() % 50 / 10.f;
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
	mHeightMap = new float *[mVMapSize];
	for (i = 0; i < mVMapSize; i++)
	{
		mHeightMap[i] = new float[mHMapSize];
		for (j = 0; j < mHMapSize; j++)
		{
			int *ptr = (int *)((BYTE *)rect.pBits + ((mVMapSize - i - 1) * rect.Pitch)) + j;
			mHeightMap[i][j] = *ptr / (float)mMaxHeight * mHeight;
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
	LPD3DXMESH mesh;
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
	D3DXCreateMeshFVF(mNumTriangles, mNumVertices, D3DPOOL_DEFAULT, D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1, d3dObj->mD3DDevice, &mesh);

	{
		float *ptr = NULL;
		mesh->LockVertexBuffer(0, (void **)&ptr);
		for (int i = 0; i < mVMapSize; i++)
			for (int j = 0; j < mHMapSize; j++)
			{
				int k = (i * mHMapSize + j) * 9;
				ptr[k + 0] = (float)j;
				ptr[k + 1] = mHeightMap[i][j];
				ptr[k + 2] = (float)i;
				float s = mHeightMap[i][j] / mHeight * (mColorNum - 1);
				((D3DCOLOR *)ptr)[k + 6] =
					mColorNum == 1 ? mColors[0] : lerp(mColors[(int)s], mColors[(int)s + 1], s - (int)s);
				ptr[k + 7] = i / 10.f;
				ptr[k + 8] = j / 10.f;
			}
		mesh->UnlockVertexBuffer();
	}

	{
		short *ptr = NULL;
		mesh->LockIndexBuffer(0, (void **)&ptr);
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
		mesh->UnlockIndexBuffer();
	}
	DWORD *adjacency = new DWORD[3 * mesh->GetNumFaces()];
	mesh->GenerateAdjacency(0.f, adjacency);
	D3DXComputeNormals(mesh, adjacency);
	float *vertexWeights = new float[mNumVertices];
	for (int i = 0; i < mVMapSize; i++)
		for (int j = 0; j < mHMapSize; j++)
			vertexWeights[i * mHMapSize + j] = (i == 0 || i == mVMapSize - 1 || j == 0 || j == mHMapSize - 1 ? 1000.f : 1.f);
	D3DXSimplifyMesh(mesh, adjacency, NULL, vertexWeights, mNumVertices / 10, D3DXMESHSIMP_VERTEX, &mMesh);
//	mMesh = mesh;
	mesh->Release();
	delete [] adjacency;
	delete [] vertexWeights;
}

float CHeightMap::height(float aX, float aZ) const
{
	if (aX < 0) aX = 0;
	if (aX >= mHMapSize - 1) aX = mHMapSize - 1.01f;
	if (aZ < 0) aZ = 0;
	if (aZ >= mVMapSize - 1) aZ = mVMapSize - 1.01f;
	float sX  = aX - (int)aX;
	float sY  = aZ - (int)aZ;
	float val1 = (1.f - sX) * mHeightMap[(int)aZ][(int)aX] + sX * mHeightMap[(int)aZ][(int)aX + 1];
	float val2 = (1.f - sX) * mHeightMap[(int)aZ + 1][(int)aX] + sX * mHeightMap[(int)aZ + 1][(int)aX + 1];
	return ((1.f - sY) * val1 + sY * val2);
}

D3DXVECTOR3 CHeightMap::mouseCoords(int aMouseX, int aMouseY)
{
	D3DXMATRIX matProj;
	d3dObj->mD3DDevice->GetTransform(D3DTS_PROJECTION, &matProj);

	// Compute the vector of the pick ray in screen space
	D3DXVECTOR3 v;
	v.x =  ((2.0f * aMouseX ) / d3dObj->width() - 1) / matProj._11;
	v.y = -((2.0f * aMouseY ) / d3dObj->height() - 1) / matProj._22;
	v.z =  1.0f;

	// Get the inverse view matrix
	D3DXMATRIX matView, m;
	d3dObj->mD3DDevice->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&m, NULL, &matView);

	// Transform the screen space pick ray into 3D space
	D3DXVECTOR3 dir, pos;
	dir.x  = v.x*m._11 + v.y*m._21 + v.z*m._31;
	dir.y  = v.x*m._12 + v.y*m._22 + v.z*m._32;
	dir.z  = v.x*m._13 + v.y*m._23 + v.z*m._33;
	pos.x = m._41;
	pos.y = m._42;
	pos.z = m._43;
	if (dir.x == 0 && dir.z == 0)
		return D3DXVECTOR3(pos.x, height(pos.x, pos.z), pos.z);
	if ((pos.x < 0 && dir.x > 0) || (pos.x >= mHMapSize && dir.x < 0))
		pos += dir * ((pos.x < 0 ? 0 : mHMapSize) - pos.x) / dir.x;
	if ((pos.z < 0 && dir.z > 0) || (pos.z >= mVMapSize && dir.z < 0))
		pos += dir * ((pos.z < 0 ? 0 : mVMapSize) - pos.z) / dir.z;
	float s = 1.f / max(abs(dir.x), abs(dir.z));
	while (pos.y > height(pos.x, pos.z))
	{
		pos += dir * s;
		if (pos.x < 0 || pos.x > mHMapSize || pos.z < 0 || pos.z > mVMapSize)
			return D3DXVECTOR3(pos.x, height(pos.x, pos.z), pos.z);
	}
	pos -= dir * ((pos.y - height(pos.x, pos.z)) / dir.y);
	return D3DXVECTOR3(pos.x, height(pos.x, pos.z), pos.z);
}

LPD3DXMESH CHeightMap::mesh()
{
	return mMesh;
}

int CHeightMap::horizontalSize() const
{
	return mHMapSize;
}

int CHeightMap::verticalSize() const
{
	return mVMapSize;
}
