#ifndef HEIGHT_MAP_H
#define HEIGHT_MAP_H

#include "animation.h"

/**
 * CHeightMap
 * @todo Use progressive mesh to optimize.
 * @todo Divide in two submeshes (plain and rough terrain).
 */
class CHeightMap : public MAnimation
{
public:
	CHeightMap(int aHMapSize, int aVMapSize);
	CHeightMap(const char *aFileName);
	~CHeightMap();
	void draw(uint32 aTime);
	void release();
	void restore(const char *aFileName);
	float height(float aX, float aZ) const;
	int horizontalSize() const;
	int verticalSize() const;
	D3DXVECTOR3 mouseCoords(int aMouseX, int aMouseY);
	LPD3DXMESH mesh();
private:
	int mHMapSize, mVMapSize;
	float **mHeightMap;
	float mHeight;
	int mMaxHeight;
	int mColorNum;
	D3DCOLOR *mColors;
	CTexture *mTexture;
	D3DMATERIAL9 mMaterial;
	LPD3DXMESH mMesh;
	int mNumVertices, mNumTriangles;
};

#endif // HEIGHT_MAP_H
