#include "../include/game.h"
#include "../include/animationStorage.h"
#include "../include/directInput.h"

inline DWORD FtoDW(FLOAT f) {return *((DWORD*)&f);}

CGame *game = NULL;

CGame::CGame() : mBuildings(256), mTime(0)
{
	game = this;
}

CGame::~CGame()
{
	mBuildings.removeAndDestroyAll();
	game = NULL;
}

bool CGame::init()
{
	LPDIRECT3DDEVICE9 device = d3dObj->mD3DDevice;
	// Set the projection matrix
	D3DXMATRIX projectionMatrix;
	ZeroMemory(&projectionMatrix, sizeof(projectionMatrix));
	float screenAspect=(float)d3dObj->width() / (float)d3dObj->height();
	float fov = D3DX_PI / 4;
	D3DXMatrixPerspectiveFovLH(&projectionMatrix, fov, screenAspect, 1.0f, 150.0f);
	device->SetTransform(D3DTS_PROJECTION, &projectionMatrix);

	// Turn	on the zbuffer
	device->SetRenderState(D3DRS_ZENABLE, TRUE);
	// Turn	on ambient lighting	
//	device->SetRenderState(D3DRS_AMBIENT, 0xffffffff);

	// General particle render states
	device->SetRenderState(D3DRS_POINTSCALE_A, FtoDW(0.0f));
	device->SetRenderState(D3DRS_POINTSCALE_B, FtoDW(1.0f));
    device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
    device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

    D3DLIGHT9 light;
    D3DXVECTOR3 vecLightDirUnnormalized(0.0f, -1.0f, 0.4f);
    ZeroMemory( &light, sizeof(D3DLIGHT9) );
    light.Type        = D3DLIGHT_DIRECTIONAL;
	light.Ambient.r = 0.2f;
	light.Ambient.g = 0.2f;
	light.Ambient.b = 0.2f;
    light.Diffuse.r = 0.8f;
    light.Diffuse.g = 0.8f;
    light.Diffuse.b = 0.8f;
    D3DXVec3Normalize((D3DXVECTOR3*)&light.Direction, &vecLightDirUnnormalized);
/*    light.Position.x = 5.0f;
    light.Position.y = -10.0f;
    light.Position.z = 5.0f;
    light.Range      = 1000.0f;
*/
    device->SetLight(0, &light);
    device->LightEnable(0, TRUE);

	D3DXQUATERNION quat;
	MAnimation *anim;

//	cam = new CCamera(3, 6969, &D3DXVECTOR3(32, 70, -15), D3DXQuaternionRotationYawPitchRoll(&quat, 0, 3.14f/3.f, 0));
//	cam = new CCamera(3, 6969, &D3DXVECTOR3(32, 25, -5), D3DXQuaternionRotationYawPitchRoll(&quat, 0, 3.14f/3.f, 0));
	cam = new CCamera(3, 6969, 10, 54, -2, 39, &D3DXVECTOR3(10, 20, 0), D3DXQuaternionRotationYawPitchRoll(&quat, 0, 3.14f/3.f, 0));

	mHeightMap = new CHeightMap("data/map.bmp");

	anim = CAnimationStorage::ptr()->getAnimation("../tools/particleEditor/particleEffects/flame_small.lua");
	CDrawable *fire = new CDrawable(0, mBuildings.firstEmpty(), anim, 1.f, &D3DXVECTOR3(2, mHeightMap->height(2, 6), 6), D3DXQuaternionRotationYawPitchRoll(&quat, 3.14f/2.f, 0, 0));
	mBuildings.add(fire);

	anim = CAnimationStorage::ptr()->getAnimation("data/tiger.x");
	CDrawable *tiger = new CDrawable(1, mBuildings.firstEmpty(), anim, 1.f, &D3DXVECTOR3(8, mHeightMap->height(8, 8) + .7f, 8), D3DXQuaternionRotationYawPitchRoll(&quat, 3.14f/2.f, 0, 0));
	mBuildings.add(tiger);

	anim = CAnimationStorage::ptr()->getAnimation("data/bones_move.x");
	CDrawable *ufo = new CDrawable(2, mBuildings.firstEmpty(), anim, 1.f, &D3DXVECTOR3(5, mHeightMap->height(5, 5), 5), D3DXQuaternionRotationYawPitchRoll(&quat, 3.14f/2.f, 0, 0));
	mBuildings.add(ufo);

	mMessageBox = new CMessageBox(d3dObj->mD3DDevice, 0, 0, 200, 200, 10);

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

	directInput->ReadState();
	float up = (float)(directInput->isPressed(MOVE_UP) - directInput->isPressed(MOVE_DOWN)) * 0.4f;
	float right = (float)(directInput->isPressed(MOVE_RIGHT) - directInput->isPressed(MOVE_LEFT)) * 0.4f;
	if (up != 0 || right != 0)
		cam->scroll(right, up);

	// Draw everything
	cam->transform();
	device->BeginScene();
	mHeightMap->draw(0);

	for (uint16 i = mBuildings.first(); i != mBuildings.end(); i = mBuildings.mTable[i].mNext)
		mBuildings.mTable[i].mObj->draw(10);
	mMessageBox->draw(device);

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

MGameObj *CGame::getGameObj(uint16 aIndex, uint16 aId)
{
	MGameObj *obj;
	int list = aIndex >> 12;
	if (list == 0)
		obj = mBuildings.object(aIndex & 0xFFF);
	if (obj && obj->id() == aId)
		return obj;
	return NULL;
}
