#ifndef WEAPONDATA_H
#define WEAPONDATA_H

#include <string>
#include <vector>
#include "animation.h"

class CWeaponData
{
public:
	CWeaponData();
	~CWeaponData();

	uint32 *getRequires() const;
	uint32 getNumRequires() const;
	uint32 getId() const;
	std::string getName() const;
	MAnimation *getModel() const;
	MAnimation *getBullet() const;
	CTexture *getPicture() const;
	uint32 getPriceMineral() const;
	uint32 getPriceGas() const;
	uint32 getMass() const;
	uint32 getRange() const;
	uint32 getTime() const;
	uint32 getType() const;
	uint32 getPiercingDamage() const;
	uint32 getExplosiveDamage() const;
	uint32 getAccuracy() const;
	int getShortcut() const;

	void setRequires(std::vector<uint32> &aRequires);
	void setId(uint32 aId);
	void setName(std::string aName);
	void setModel(MAnimation *aModel);
	void setBullet(MAnimation *aBullet);
	void setPicture(CTexture *aPicture);
	void setPriceMineral(uint32 aPriceMineral);
	void setPriceGas(uint32 aPriceGas);
	void setMass(uint32 aMass);
	void setRange(uint32 aRange);
	void setTime(uint32 aTime);
	void setType(uint32 aType);
	void setPiercingDamage(uint32 aPiercingDamage);
	void setExplosiveDamage(uint32 aExplosiveDamage);
	void setAccuracy(uint32 aAccuracy);
	void setShortcut(int aShortcut);

private:
	uint32 *mRequires;
	uint32 mNumRequires;
	uint32 mId;
	std::string mName;
	MAnimation *mModel;
	MAnimation *mBullet;
	CTexture *mPicture;
	uint32 mPriceMineral;
	uint32 mPriceGas;
	uint32 mMass;
	uint32 mRange;
	uint32 mTime;
	uint32 mType;
	uint32 mPiercingDamage;
	uint32 mExplosiveDamage;
	uint32 mAccuracy;
	int mShortcut;
};

#endif /* WEAPONDATA_H */