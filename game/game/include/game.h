#ifndef GAME_H
#define GAME_H

#include "particleSystem.h"

class CGame
{
public:
	CGame();
	~CGame();
	bool init();
	bool loop();
private:
	CParticleSystem *mPs;
	int mTime;
};

#endif // GAME_H