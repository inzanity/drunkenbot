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

	CSetValue *getSetValues();
	uint32 getNumSetValues();
	uint32 getPriceGas();
	uint32 getPriceMineral();

	void setSetValues(std::vector<CSetValue *> &aSetValues);
	void setPriceGas(uint32 aPriceGas);
	void setPriceMineral(uint32 aPriceMineral);

private:
	CSetValue *mSetValues;
	uint32 mNumSetValues;
	uint32 mPriceGas;
	uint32 mPriceMineral;
};

#endif