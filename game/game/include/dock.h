#ifndef DOCK_H
#define DOCK_H

#include "building.h"

class CDock : public CBuilding
{
public:
	CDock(CGameObjPtr aObjPtr, bool aReady, const D3DXVECTOR3 *aPos, float aXAngle, float aYAngle);
	CDock(CGameObjPtr aObjPtr, CBuildingData *aBuildingData, bool aReady, const D3DXVECTOR3 *aPos, float aXAngle, float aYAngle);
	CDock(istream &aStream);
	virtual			~CDock();

	virtual	void	externalize		(ostream &aStream);
	virtual	void	handleMessage	(CMessage *aMsg);
private:
};

#endif // DOCK_H

