#ifndef BUILDINGDATA_H
#define BUILDINGDATA_H

#include "animation.h"

class CBuildingData
{
public:
	/**
	 * Constructor with all the needed data.
	 * These will be constructed with
	 * the xml-parser, all information is constant.
	 *
	 * @param aAnimation Animation of the building.
	 * @param aConstructAnimation Constructing animation.
	 * @param aHitPoints Amount of hitpoints for this type of building.
	 * @param aPrice Price of buildingtype.
	 * @param aReqMask Bitmask defining the required technologies.
	 */
	CBuildingData(MAnimation *aAnimation, MAnimation *aConstructAnimation, int aHitpoints, int aPrice, int aTechnologyMask);
	/**
	 * Destructor.
	 */
	~CBuildingData();
	/**
	 * Getter for the animation file path.
	 */
	MAnimation *getAnimation();
	/**
	 * Getter for the constructing animation file path.
	 */
	MAnimation *getConstructAnimation();
	/**
	 * Getter for construction time.
	 */
	uint32 getConstructionTime() const;
	/**
	 * Getter for hitpoints.
	 */
	int getHitpoints() const;
	/**
	 * Getter for price.
	 */
	int getPrice() const;
	/**
	 * Getter for the technology mask.
	 */
	int getTechnologyMask() const;

private:
	MAnimation *mAnimation;
	MAnimation *mConstructAnimation;
	uint32 mConstructionTime;
	int mHitpoints;
	int mPrice;
	int mTechnologyMask;
};

#endif // BUILDINGDATA_H
