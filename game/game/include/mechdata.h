#ifndef MECHDATA_H
#define MECHDATA_H

#include "animation.h"

class CMechData
{
public:
	CMechData();
	~CMechData();

	void setPriceMineral(uint32 aPriceMineral);
	void setPriceGas(uint32 aPriceGas);
	void setHitpoints(uint32 aHitpoints);
	void setPiercingArmor(uint32 aPiercingArmor);
	void setExplosiveArmor(uint32 aExplosiveArmor);
	void setSpeed(uint32 aSpeed);
	void setAnimation(MAnimation *aAnimation);
	void setMass(uint32 aMass);

	uint32 getPriceMineral() const;
	uint32 getPriceGas() const;
	uint32 getHitpoints() const;
	uint32 getPiercingArmor() const;
	uint32 getExplosiveArmor() const;
	uint32 getSpeed() const;
	MAnimation *getAnimation() const;
	uint32 getMass() const;

private:
	uint32 mPriceMineral;
	uint32 mPriceGas;
	uint32 mHitpoints;
	uint32 mPiercingArmor;
	uint32 mExplosiveArmor;
	uint32 mSpeed;
	MAnimation *mAnimation;
	uint32 mMass;
};

#endif /* MECHDATA_H */