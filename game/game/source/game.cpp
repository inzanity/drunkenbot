#include "../include/game.h"
#include "../include/particleSystemLoader.h"

CGame::CGame() : mBuildings(1000), mTime(0)
{
}

CGame::~CGame()
{
	mBuildings.removeAndDestroyAll();
}

bool CGame::init()
{
	LPDIRECT3DDEVICE9 device = d3dObj->mD3DDevice;
	// Set the projection matrix
	D3DXMATRIX projectionMatrix;
	ZeroMemory(&projectionMatrix, sizeof(projectionMatrix));
	float screenAspect=(float)d3dObj->width() / (float)d3dObj->height();
	float FOV = D3DX_PI / 4;
	D3DXMatrixPerspectiveFovLH(&projectionMatrix, FOV, screenAspect, 1.0f, 150.0f);
	device->SetTransform(D3DTS_PROJECTION, &projectionMatrix);

	MAnimation *anim = CParticleSystemLoader::load("e:/programming/projects/game/tools/particleEditor/particleEffects/flame.lua");
	CDrawable *building = new CDrawable(0, mBuildings.firstEmpty(), anim, 1.f, &D3DXVECTOR3(0, 0, 20), &D3DXQUATERNION());
	mBuildings.add(building);
	cam = new CCamera(1, 6969);
	mTime = 0;

	return true;
}

bool CGame::loop()
{
	LPDIRECT3DDEVICE9 device = d3dObj->mD3DDevice;
	mTime += 30; // TODO: Real timing system
	device->Clear(0,0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0,0,0), 1.0f, 0);

//	mMsgList.sendMessages(mTime, &mObjList);
	chkDestroyList();

	// Draw everything
	cam->transform();
	device->BeginScene();

	for (uint16 i = mBuildings.first(); i != mBuildings.end(); i = mBuildings.mTable[i].mNext)
		mBuildings.mTable[i].mObj->draw(10);

	device->EndScene();
	d3dObj->flip();
	return true;
}

void CGame::sendMessage(CMessage *aMsg)
{
	aMsg->mTime += mTime;
	mMsgList.add(aMsg);
}

void CGame::sendMessage(uint16 aMsg, uint16 aSenderId, uint16 aReceiverId,
						uint16 aSender, uint16 aReceiver,
						uint32 aParam1, uint32 aParam2, uint32 aTime)
{
	mMsgList.add(new CMessage(aMsg, aSenderId, aReceiverId, aSender, aReceiver, aParam1, aParam2, mTime + aTime));
}

void CGame::sendMessage(uint16 aMsg, MGameObj *aObj, uint32 aParam1, uint32 aParam2, uint32 aTime)
{
	mMsgList.add(new CMessage(aMsg, aObj->id(), aObj->id(), aObj->index(),
		aObj->index(), aParam1, aParam2, mTime + aTime));
}

void CGame::destroyObj(MGameObj *aObj)
{
	mDestroyList.push_back(aObj);
}

void CGame::chkDestroyList()
{
	list<MGameObj *>::iterator iter;
	while (!mDestroyList.empty())
	{
//		objectList()->remove(iter->m_object->index());
		delete mDestroyList.back();
		mDestroyList.pop_back();
	}
}
