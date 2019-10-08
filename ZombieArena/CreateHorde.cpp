#include "ZombieArena.h"
#include "Zombie.h"

Zombie* createHorde(int numZombies, IntRect arena) {

	Zombie* zombies = new Zombie[numZombies];

	int maxY = arena.height - 20;//bottom
	int minY = arena.top + 20;//top
	int maxX = arena.width - 20;//right
	int minX = arena.left + 20;//left

	for (int i = 0; i < numZombies; i++) {
		// Which side should the zombie spawn
		srand((int)time(0) * i);
		int side = (rand() % 4);//between 0 and 3
		float x, y;//current zombie spawn position

		switch (side) {
		case 0:
			// left
			x = minX;
			y = (rand() % maxY) + minY;//between the start and end of the left wall
			break;
		case 1:
			// right
			x = maxX;
			y = (rand() % maxY) + minY;
			break;
		case 2:
			// top
			x = (rand() % maxX) + minX;
			y = minY;
			break;
		case 3:
			// bottom
			x = (rand() % maxX) + minX;
			y = maxY;
			break;
		}
		// Bloater, crawler or runner
		srand((int)time(0) * i * 2);
		int type = (rand() % 3);//between 0 and 2
		// Spawn the new zombie into the array
		zombies[i].spawn(x, y, type, i);
	}//end for
	/*we are actually returning the address of the first element of the array.
	This is the same thing as a pointer.*/
	return zombies;
}