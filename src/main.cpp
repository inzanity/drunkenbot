#include <iostream>
#include <fstream>
#include "../inc/engine.h"

using std::cout;
using std::endl;
using std::ifstream;

int main()
{
	ifstream map("res/map.txt");
	ifstream weapons("res/weapons.txt");

	CGameEngine *engine = new CGameEngine(&weapons, &map);
	while (engine->loop());
	char **results = engine->getResults(false);
	for (int i = 0; results[i]; i++)
		cout << results[i] << endl;
	return 1;
}
