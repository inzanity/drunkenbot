#ifndef TURRET_H
#define TURRET_H
#include "building.h"
#include <time.h>

enum TTurretMessage{EMsgActivate = EMsgBuildingLast};

class CTurret : public CBuilding
{
public:
	CTurret(CGameObjPtr aObjPtr, bool aReady, const D3DXVECTOR3 *aPos, float aXAngle, float aYAngle);
	CTurret(CGameObjPtr aObjPtr, CBuildingData *aBuildingData, bool aReady, const D3DXVECTOR3 *aPos, float aXAngle, float aYAngle);
	CTurret(istream &aStream);
	virtual			~CTurret();

	virtual	void	externalize		(ostream &aStream);
	virtual	void	handleMessage	(CMessage *aMsg);
	virtual	void	update			(uint32 aTimeFactor);

	// returns the angle of turretTop
	float			angle			();
	// returns true, if ready to shoot
	bool			active			();
	// draws turret and turretTop
	void			draw(uint32 aTimeFactor);

private:
	float			mTurretAngle, mTargetAngle;
	time_t			mTime;
	bool			mTargetReached;
};

#endif // TURRET_H
