#ifndef TECHNOLOGYDATA_H
#define TECHNOLOGYDATA_H

#include <vector>
#include <string>
#include "animation.h"

class CTechnologyData {
public:
	CTechnologyData();
	~CTechnologyData();

	void setRequires(std::vector<uint32> &aRequires);
	void setId(uint32 aId);
	void setName(std::string aName);
	void setPriceMineral(uint32 aPriceMineral);
	void setPriceGas(uint32 aPriceGas);
	void setDevelopTime(uint32 aDevelopTime);
	void setPicture(CTexture *aPicture);

	uint32 *getRequires() const;
	uint32 getNumRequires() const;
	uint32 getId() const;
	std::string getName() const;
	uint32 getPriceMineral() const;
	uint32 getPriceGas() const;
	uint32 getDevelopTime() const;
	CTexture *getPicture() const;

private:
	uint32 *mRequires;
	uint32 mNumRequires;
	uint32 mId;
	std::string mName;
	uint32 mPriceMineral;
	uint32 mPriceGas;
	uint32 mDevelopTime;
	CTexture *mPicture;
};

#endif /* TECHNOLOGYDATA_H */