#ifndef GAMEUI_H
#define GAMEUI_H

#include "gameObj.h"

class MGameUI
{
public:
	virtual ~MGameUI() {}
	virtual void draw(uint32 aTime) = 0;
	virtual void handleInput() = 0;
	virtual void activate(CGameObjPtr aTarget) = 0;
};

#endif // GAMEUI_H
