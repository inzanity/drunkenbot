#include "../include/game.h"
#include "../include/animationStorage.h"
#include "../include/directInput.h"
#include "../include/turret.h"
#include "../include/dock.h"

#pragma comment(lib, "winmm")

inline DWORD FtoDW(FLOAT f) {return *((DWORD*)&f);}

CGame *game = NULL;

CGame::CGame() : mBuildings(64), mMechs(32), mExplosions(32), mTime(0), mNewId(0), mPrevTime(timeGetTime()), mShowFPS(1)
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
	// Turn	on the zbuffer
	device->SetRenderState(D3DRS_ZENABLE, TRUE);
	// Turn	on ambient lighting	
//	device->SetRenderState(D3DRS_AMBIENT, 0xffffffff);

	// General particle render states
	device->SetRenderState(D3DRS_POINTSCALE_A, FtoDW(0.0f));
	device->SetRenderState(D3DRS_POINTSCALE_B, FtoDW(1.0f));
    device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
    device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

    D3DLIGHT9 light;
    D3DXVECTOR3 vecLightDirUnnormalized(0.0f, -1.0f, 0.4f);
    ZeroMemory(&light, sizeof(D3DLIGHT9));
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

	mNumPlayers = 2;
	mPlayers = new CPlayer *[mNumPlayers];
	mPlayers[0] = new CPlayer(NULL, 0);
	mPlayers[1] = new CPlayer(NULL, 0);
	mPlayer = 0;
	mHeightMap = new CHeightMap("data/map.bmp");
	mRTSModeUI = new CRTSModeUI(mPlayers[mPlayer]);
	mFPSModeUI = new CFPSModeUI();

	// Set the projection matrix
	D3DXMATRIX projectionMatrix;
	float screenAspect=(float)d3dObj->width() / (float)d3dObj->height();
	float fov = D3DX_PI / 4;
	D3DXMatrixPerspectiveFovLH(&projectionMatrix, fov, screenAspect, 1.0f, 150.0f);
	device->SetTransform(D3DTS_PROJECTION, &projectionMatrix);

	D3DXQUATERNION quat;
	MAnimation *anim;

//	mCam = new CCamera(getNewGameObjectPtr(ETypeCamera), 10, 54, -10, 39, &D3DXVECTOR3(32, 50, -15), D3DXQuaternionRotationYawPitchRoll(&quat, 0, 3.14f/3.f, 0));
//	mCam = new CCamera(getNewGameObjectPtr(ETypeCamera), 10, 54, -2, 39, &D3DXVECTOR3(32, 25, -5), D3DXQuaternionRotationYawPitchRoll(&quat, 0, 3.14f/3.f, 0));
	mCam = new CCamera(getNewGameObjectPtr(ETypeCamera), 10, 54, -4, 39, &D3DXVECTOR3(10, 20, 0), D3DXQuaternionRotationYawPitchRoll(&quat, 0, 3.14f/3.f, 0));

	mPathFinder			= new CPathFinder(mHeightMap->mesh());

	anim = CAnimationStorage::ptr()->getAnimation("../tools/particleEditor/particleEffects/flame_small.lua");
	CDrawable *fire		= new CDrawable(getNewGameObjectPtr(ETypeBuilding), anim, 1.f, &D3DXVECTOR3(2, mHeightMap->height(2, 6), 6), D3DXQuaternionRotationYawPitchRoll(&quat, 3.14f/2.f, 0, 0));
	mBuildings.add(fire);

	CMech *mech			= new CMech(getNewGameObjectPtr(ETypeMech), &D3DXVECTOR3(5, mHeightMap->height(5, 5), 5), D3DXQuaternionRotationYawPitchRoll(&quat, 0, 0, 0));
	mMechs.add(mech);

	CTurret *turret	= new CTurret(getNewGameObjectPtr(ETypeBuilding), true, &D3DXVECTOR3(10, mHeightMap->height(10, 10), 10), D3DXQuaternionRotationYawPitchRoll(&quat, 0, 1, 0));
	mBuildings.add(turret);
	turret = new CTurret(getNewGameObjectPtr(ETypeBuilding), true, &D3DXVECTOR3(10, mHeightMap->height(10, 20), 20), D3DXQuaternionRotationYawPitchRoll(&quat, 0, 1, 0));
	mBuildings.add(turret);
	turret = new CTurret(getNewGameObjectPtr(ETypeBuilding), true, &D3DXVECTOR3(40, mHeightMap->height(40, 40), 40), D3DXQuaternionRotationYawPitchRoll(&quat, 0, 1, 0));
	mBuildings.add(turret);
	turret	= new CTurret(getNewGameObjectPtr(ETypeBuilding), true, &D3DXVECTOR3(10, mHeightMap->height(10, 50), 50), D3DXQuaternionRotationYawPitchRoll(&quat, 0, 1, 0));
	mBuildings.add(turret);

	CDock *dock1		= new CDock(getNewGameObjectPtr(ETypeBuilding), true, &D3DXVECTOR3(35, mHeightMap->height(35, 10), 10), D3DXQuaternionRotationYawPitchRoll(&quat, 0, 1, 0));
	mBuildings.add(dock1);

	mMessageBox = new CMessageBox(0, 0, 200, 200, 10);

	mRTSModeUI->activate(CGameObjPtr());
	mTime = 0;

	return true;
}

bool CGame::loop()
{
	int i;
	LPDIRECT3DDEVICE9 device = d3dObj->mD3DDevice;
	mTime += 10; // TODO: Real timing system
	device->Clear(0,0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0,0,0), 1.0f, 0);

	mMsgList.sendMessages(mTime);
	chkDestroyList();

	directInput->ReadState();
	float up = (float)(directInput->isPressed(MOVE_UP) - directInput->isPressed(MOVE_DOWN)) * 0.04f;
	float right = (float)(directInput->isPressed(MOVE_RIGHT) - directInput->isPressed(MOVE_LEFT)) * 0.04f;
	D3DXQUATERNION quat;
	if (up != 0 || right != 0)
		mCam->scroll(right, up);

	const DIMOUSESTATE2 *mouse = directInput->getMouseState();

	// Draw everything
	mCam->transform(10);
	device->BeginScene();

//	device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	mHeightMap->draw(0);
//	device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	mRTSModeUI->handleInput();
	mFPSModeUI->handleInput();
/*	if (directInput->checkMouseButton(0))
	{
		static int find = 0;
		static D3DXVECTOR3 start;
		D3DXVECTOR3 v = mHeightMap->mouseCoords(directInput->getMouseX(), directInput->getMouseY());
		char temp[64];
		sprintf(temp, "%f, %f, %f", v.x, v.y, v.z);
		mMessageBox->addMessage(temp, mTime + 200*10);
		if (find == 0)
		{
			start = v;
			uint16 ind = mPathFinder->getIndex(&v);
			mPathFinder->drawTriangle(ind, D3DCOLOR_XRGB(255, 0, 0));
			find = 1;
		}
		else
		{
			find = 0;
			mPathFinder->findPath(&start, &v);
		}
	}
*/
//	mPathFinder->drawPath();

//	device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
//	mHeightMap->draw(0);
//	device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	for (i = mMechs.first(); i != mMechs.end(); i = mMechs.mTable[i].mNext)
		mMechs.mTable[i].mObj->update(10);

	for (i = mBuildings.first(); i != mBuildings.end(); i = mBuildings.mTable[i].mNext)
	{
		mBuildings.mTable[i].mObj->draw(10);
		mBuildings.mTable[i].mObj->update(2);
	}
	MGameObj *fpsTarget = mCam->targetObj().ptr();
	for (i = mMechs.first(); i != mMechs.end(); i = mMechs.mTable[i].mNext)
		if (mMechs.mTable[i].mObj != fpsTarget) //dont draw the mech currently in control
			mMechs.mTable[i].mObj->draw(10);

	if (directInput->checkKey(WRITE_TEXT))
		mMessageBox->addMessage("foo", 10);
	mRTSModeUI->draw(mTime);
	mFPSModeUI->draw(mTime);
	if (mShowFPS)
		mShowFPS++;
	if (mShowFPS > 20)
	{
		DWORD time = timeGetTime() - mPrevTime;
		mPrevTime = timeGetTime();
		char temp[64];
		sprintf(temp, "FPS: %f", 20000.f / time);
//		mMessageBox->addMessage(temp, mTime + 20*10);
//		sprintf(temp, "mouse1: %d, mouse2: %d, mouse3: %d", (int)mouse->rgbButtons[0], (int)mouse->rgbButtons[1], (int)mouse->rgbButtons[2]);
//		mMessageBox->addMessage(temp, mTime + 20*10);
		mShowFPS = 1;
	}

	mMessageBox->draw(mTime);
	d3dObj->mD3DDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	for (i = mExplosions.first(); i != mExplosions.end(); i = mExplosions.mTable[i].mNext)
		mExplosions.mTable[i].mObj->draw(10);
	d3dObj->mD3DDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);

	device->EndScene();
	d3dObj->flip();
	return true;
}

void CGame::sendMessage(CMessage *aMsg)
{
	aMsg->mTime += mTime;
	mMsgList.add(aMsg);
}

void CGame::sendMessage(uint16 aMsg, CGameObjPtr aSender, CGameObjPtr aReceiver,
						uint32 aParam1, uint32 aParam2, uint32 aTime)
{
	mMsgList.add(new CMessage(aMsg, aSender, aReceiver, aParam1, aParam2, mTime + aTime));
}

void CGame::sendMessage(uint16 aMsg, MGameObj *aObj, uint32 aParam1, uint32 aParam2, uint32 aTime)
{
	mMsgList.add(new CMessage(aMsg, aObj->objectPtr(), aObj->objectPtr(),
		aParam1, aParam2, mTime + aTime));
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
		CGameObjPtr objPtr = (*iter)->objectPtr();
		int objType = objPtr.index() >> 12;
		if (objType == ETypeBuilding)
			mBuildings.remove(objPtr.index());
		else if (objType == ETypeMech)
			mMechs.remove(objPtr.index());
		delete mDestroyList.back();
		mDestroyList.pop_back();
	}
}

CGameObjPtr CGame::getNewGameObjectPtr(TObjectType aObjType)
{
	uint16 index = 0;
	if (aObjType == ETypeBuilding)
		index = mBuildings.firstEmpty();
	else if (aObjType == ETypeMech)
		index = mMechs.firstEmpty();
	return CGameObjPtr(mNewId++, uint16(index | (aObjType << 12)));
}

MGameObj *CGame::getGameObj(uint16 aIndex, uint16 aId)
{
	if (aIndex == KNullIndex)
		return NULL;
	MGameObj *obj;
	int objType = aIndex >> 12;
	if (objType == ETypeBuilding)
		obj = mBuildings.object(aIndex & 0xFFF);
	else if (objType == ETypeMech)
		obj = mMechs.object(aIndex & 0xFFF);
	else if (objType == ETypeCamera)
		obj = mCam;
	if (obj && obj->objectPtr().id() == aId)
		return obj;
	return NULL;
}

TObjectType CGame::getObjType(CGameObjPtr aPtr)
{
	uint16 index = aPtr.index();
	if (index == KNullIndex)
		return ETypeNone;
	return (TObjectType)(index >> 12);
}
