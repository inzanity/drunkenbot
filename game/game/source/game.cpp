#include "../include/game.h"
#include "../include/particleSystemLoader.h"

float frand(float min, float max)
{
	return min + rand() / (float)RAND_MAX * (max - min);
}

CGame::CGame() : mPs(NULL), mTime(0)
{
}

CGame::~CGame()
{
	if (mPs)
		delete mPs;
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

	D3DXMATRIX viewMatrix;
	D3DXMatrixLookAtLH(&viewMatrix, &D3DXVECTOR3(0.0f, 0.0f, -70.0f), &D3DXVECTOR3(0.0f, 0.0f, 0.0f), &D3DXVECTOR3(0.0f,1.0f,0.0f));
	device->SetTransform(D3DTS_VIEW, &viewMatrix);

	mPs = CParticleSystemLoader::load("e:\\programming\\projects\\game\\tools\\particleEditor\\particleEffects\\simple.lua");
/*	mPs = new CParticleSystem(100, 2, 1, 1, "particle.bmp");
	for (int i = 0; i < 100; i++)
	{
		D3DXVECTOR3 pos[2] = {D3DXVECTOR3(0.f, 0.f, 0.f), D3DXVECTOR3(frand(-20.f, 20.f), frand(-20.f, 20.f), frand(-20.f, 20.f))};
		D3DCOLOR color = D3DCOLOR_XRGB(10, 10, 200);//D3DCOLOR_XRGB(rand()%0xFF, rand()%0xFF, rand()%0xFF);
		float size = frand(.1f, .5f);
		mPs->setParticle(i, 1000, pos, &color, NULL);
	}
*/	mTime = 0;

	return true;
}

bool CGame::loop()
{
	LPDIRECT3DDEVICE9 device = d3dObj->mD3DDevice;
	mTime++;
	device->Clear(0,0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0,0,0), 1.0f, 0);
	device->BeginScene();
	mPs->draw(mTime * 10);
	device->EndScene();
	d3dObj->flip();
	return true;
}
