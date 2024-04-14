#ifndef _POWERUP_INCLUDE
#define _POWERUP_INCLUDE


#include "Sprite.h"
#include "TileMap.h"





class PowerUp
{

public:



	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, int compow, int index);
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);
	glm::ivec2 getpos();
	
	int cpu;
	int in;
	vector<int> pointscomida = { 0,500,8000,16000,22000,30000,50000 };

private:


	
	int CHAR_HEIGHT = 16;
	int CHAR_WIDTH = 16;
	glm::ivec2 tileMapDispl, posPlayer;

	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;

};


#endif // _BUBBLE_INCLUDE

