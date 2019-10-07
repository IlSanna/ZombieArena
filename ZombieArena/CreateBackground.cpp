#include "ZombieArena.h"

int createBackground(VertexArray& rVA, IntRect arena) {
	const int TILE_SIZE = 50;
	const int TILE_TYPES = 3;
	const int VERTS_IN_QUAD = 4;

	int worldWidth = arena.width / TILE_SIZE;//number of tiles on width
	int worldHeight = arena.height / TILE_SIZE;//number of tiles on height

	rVA.setPrimitiveType(Quads);//set primitive
	rVA.resize(worldWidth * worldHeight * VERTS_IN_QUAD);

	int currentVertex = 0;//start from the first vertex of vertex array

	for (int w = 0; w < worldWidth; w++) {
		for (int h = 0; h < worldHeight; h++) {
			//set 4 vertices per quad
			rVA[currentVertex + 0] = Vector2f(w * TILE_SIZE,h * TILE_SIZE);//0,0
			rVA[currentVertex + 1] = Vector2f((w * TILE_SIZE) + TILE_SIZE,h * TILE_SIZE);//(1,0)
			rVA[currentVertex + 2] = Vector2f(w * TILE_SIZE + TILE_SIZE, (h * TILE_SIZE) + TILE_SIZE);//(1,1)
			rVA[currentVertex + 3] = Vector2f(w * TILE_SIZE, (h * TILE_SIZE) + TILE_SIZE);//(0,1)
			
			//UVs
			//define which texture to apply to each quad
			//if is on the bottom,top and left,right 
			if (w == 0 || h == 0 || w == worldWidth - 1 || h == worldHeight - 1) {
				//assign the appropriate corner of the wall texture within the sprite sheet
				rVA[currentVertex + 0].texCoords = Vector2f(0, 0 + TILE_TYPES * TILE_SIZE);//0,0
				rVA[currentVertex + 1].texCoords = Vector2f(TILE_SIZE, 0 + TILE_TYPES * TILE_SIZE);//(1,0)
				rVA[currentVertex + 2].texCoords = Vector2f(TILE_SIZE, TILE_SIZE + TILE_TYPES * TILE_SIZE);//(1,1)
				rVA[currentVertex + 3].texCoords = Vector2f(0, TILE_SIZE + TILE_TYPES * TILE_SIZE);//(0,1)
			}
			else {
				// Use a random floor texture
				srand((int)time(0) + h * w - h);
				int mOrG = (rand() % TILE_TYPES);//mud or grass, 0 to tile_types
				int verticalOffset = mOrG * TILE_SIZE;//so we can access the right tile this loop

				rVA[currentVertex + 0].texCoords = Vector2f(0, 0 + verticalOffset);//0,0
				rVA[currentVertex + 1].texCoords = Vector2f(TILE_SIZE, 0 + verticalOffset);//(1,0)
				rVA[currentVertex + 2].texCoords = Vector2f(TILE_SIZE, TILE_SIZE+ verticalOffset);//(1,1)
				rVA[currentVertex + 3].texCoords = Vector2f(0, TILE_SIZE + verticalOffset);//(0,1)
			}
			currentVertex += VERTS_IN_QUAD;
		}//end inner for loop
	}//end outer for loop

	return TILE_SIZE;
}