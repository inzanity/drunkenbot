#ifndef DOCK_H
#define DOCK_H

#include "building.h"

class CDock : public CBuilding
{
public:
	CDock(CGameObjPtr aObjPtr, bool aReady, const D3DXVECTOR3 *aPos, const D3DXQUATERNION *aOrientation = &D3DXQUATERNION(0, 0, 0, 1));
	CDock(istream &aStream);
	virtual			~CDock();

	virtual	void	externalize		(ostream &aStream);
	virtual	void	handleMessage	(CMessage *aMsg);
	virtual	void	update			(uint32 aTimeFactor);

	// draws Dock and DockTop
	void			draw(uint32 aTimeFactor);

private:
	CBuildingData	*mDock;
	float			mSize;
};

#endif // DOCK_H

