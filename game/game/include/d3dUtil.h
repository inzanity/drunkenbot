#ifndef D3D_UTIL_H
#define D3D_UTIL_H

#include <windows.h>

#include <d3d9.h>
#include <d3dx9.h>

class CD3DObj
{
public:
	CD3DObj					(HWND aHWnd);
	~CD3DObj				();
	bool	initFullScreen	(int aWidth, int aHeight, D3DFORMAT aFormat = D3DFMT_A8R8G8B8);
	bool	initWindowed	();
	void	release			();
	bool	reset			();
	bool	operational		();
	void	flip			();
	int		width			() const;
	int		height			() const;
private:
	bool	initFail		(char *aMsg);
	void	init			();
public:
	LPDIRECT3DDEVICE9		mD3DDevice;	// Our rendering device
	ID3DXMatrixStack *		mMatrixStack; // Matrix stack
private:
	HWND					mHWnd;
	D3DPRESENT_PARAMETERS	mD3DPresentParams;
	LPDIRECT3D9 			mD3D;
};

extern CD3DObj *d3dObj;

#endif // D3D_UTIL_H