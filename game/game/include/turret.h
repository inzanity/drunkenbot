#ifndef TURRET_H
#define TURRET_H

#include "building.h"
#include <time.h>

enum TTurretMessage{EMsgActivate = EMsgBuildingLast};

class CTurret : public CBuilding
{
public:
	CTurret(CGameObjPtr aObjPtr, bool aReady, const D3DXVECTOR3 *aPos, const D3DXQUATERNION *aOrientation = &D3DXQUATERNION(0, 0, 0, 1));
	CTurret(istream &aStream);
	virtual			~CTurret();

	virtual	void	externalize		(ostream &aStream);
	virtual	void	handleMessage	(CMessage *aMsg);
	virtual	void	update			(uint32 aTimeFactor);

	// returns the angle of turretTop
	float			angle			();
	// draws turret and turretTop
	void			draw(uint32 aTimeFactor);

private:
	D3DXQUATERNION	mDestination;
	CBuildingData	*mTurret;
	MAnimation		*mTurretTop;
	float			mTurretAngle;

	time_t			mTime;
};

#endif // TURRET_H
