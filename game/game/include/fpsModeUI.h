#ifndef FPSMODEUI_H
#define FPSMODEUI_H

#include "gameUI.h"

class CFPSModeUI : public MGameUI
{
public:
	CFPSModeUI();
	~CFPSModeUI();

	void draw(uint32 aTime);
	void handleInput();
	void activate(CGameObjPtr aTarget);
private:
	bool mActive;
	int mCounter;
	CGameObjPtr mMech;
};

#endif // FPSMODEUI_H
