#include "../include/game.h"
#include "../include/animationStorage.h"

#define GRAVITY 0.05f

CMech::CMech(CGameObjPtr aObjPtr, const D3DXVECTOR3 *aPos, const D3DXQUATERNION *aOrientation) :
	CDrawable(aObjPtr, CAnimationStorage::ptr()->getAnimation("data/bones_move.x"), 1.f, aPos, aOrientation)
{
	D3DXQuaternionRotationYawPitchRoll(&mOrientation, 0.5f, 0, 0);
	game->sendMessage(EMsgMove, this, 0, 0, 0);
	mSize = 2;
}

CMech::CMech(istream &aStream) :
	CDrawable(aStream)
{
}

CMech::~CMech()
{
}

void CMech::externalize(ostream &aStream)
{
}

void CMech::handleMessage(CMessage *aMsg)
{
	D3DXMATRIX mat;
	D3DXVECTOR4 out;
	switch (aMsg->mMsg)
	{
	case EMsgMove:
		D3DXMatrixRotationQuaternion(&mat, &mOrientation);
		D3DXVec3Transform(&out, &D3DXVECTOR3(0, 0, 0.002f), &mat);
		D3DXQuaternionIdentity(&mRotSpeed);
		mSpeed = (D3DXVECTOR3)out;
		game->sendMessage(EMsgTurnAround, this, aMsg->mParam1, 0, 30000);
		break;
	case EMsgTurnAround:
		float dir = 0.5f + (aMsg->mParam1 == 0 ? 0 : 2 * 3.14f);
		mSpeed = D3DXVECTOR3(0, 0, 0);
		D3DXQuaternionRotationYawPitchRoll(&mRotSpeed, dir/2000.f, 0, 0);
		game->sendMessage(EMsgMove, this, 1 - aMsg->mParam1, 0, 2000);
		break;
	}
}

void CMech::update(uint32 aTimeFactor)
{
	CDrawable::update(aTimeFactor);
	float h = game->mHeightMap->height(mPos.x, mPos.z);
	if (mPos.y > h + GRAVITY)
		mPos.y -= GRAVITY;
	else
		mPos.y = h;
}

// TODO
D3DXVECTOR3 CMech::getEyePos() const
{
	D3DXVECTOR3 eye = mPos;
	eye.y += mSize * 0.9f;
	return eye;
}
