#include "../include/game.h"
#include "../include/animationStorage.h"


#define GRAVITY 0.05f

CMech::CMech(CGameObjPtr aObjPtr, const D3DXVECTOR3 *aPos, const D3DXQUATERNION *aOrientation) :
	CDrawable(aObjPtr, CAnimationStorage::ptr()->getAnimation("data/bones_move.x"), 1.f, aPos, aOrientation)
{
	D3DXQuaternionRotationYawPitchRoll(&mOrientation, 0.5f, 0, 0);
	game->sendMessage(EMsgMechMove, this, 0, 0, 0);
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
	case EMsgMechMove:
		D3DXMatrixRotationQuaternion(&mat, &mOrientation);
		D3DXVec3Transform(&out, &D3DXVECTOR3(0, 0, .002f), &mat);
		D3DXQuaternionIdentity(&mRotSpeed);
		mSpeed = (D3DXVECTOR3)out;
		game->sendMessage(EMsgMechTurnAround, this, 0, 0, 20000);
		break;
	case EMsgMechTurnAround:
		mSpeed = D3DXVECTOR3(0, 0, 0);
		D3DXQuaternionRotationYawPitchRoll(&mRotSpeed, 3.1415/2000.f, 0, 0);
		game->sendMessage(EMsgMechMove, this, 0, 0, 2000);
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
