#ifndef PLAYER_H
#define PLAYER_H

#include "buildingData.h"

class CPlayer
{
public:
	CPlayer(CBuildingData **aBuildings, int aNumBuildings);
	const CBuildingData *getBuildingData(int aIndex) const;
	int numBuildings() const;
	int minerals() const;
	void setMinerals(int aMinerals);
private:
	CBuildingData **mBuildings;
	int mNumBuildings;
	int mMinerals;
};

#endif // PLAYER_H