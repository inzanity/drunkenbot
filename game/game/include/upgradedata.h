#ifndef UPGRADEDATA_H
#define UPGRADEDATA_H

#include <vector>
#include "animation.h"

class CUpgradeData
{
public:
	struct CSetValue
	{
		char *mAttribute;
		float mValue;
	};

	CUpgradeData();
	~CUpgradeData();

	uint32 getId() const;
	std::string getName() const;
	CSetValue *getSetValues() const;
	uint32 getNumSetValues() const;
	uint32 getPriceGas() const;
	uint32 getPriceMineral() const;
	CTexture *getPicture() const;
	int getShortcut() const;

	void setId(uint32 aId);
	void setName(const char *aName);
	void setSetValues(std::vector<CSetValue *> &aSetValues);
	void setPriceGas(uint32 aPriceGas);
	void setPriceMineral(uint32 aPriceMineral);
	void setPicture(CTexture *aPicture);
	void setShortcut(int aShortcut);

private:
	uint32 mId;
	std::string mName;
	CSetValue *mSetValues;
	uint32 mNumSetValues;
	uint32 mPriceGas;
	uint32 mPriceMineral;
	CTexture *mPicture;
	int mShortcut;
};

#endif