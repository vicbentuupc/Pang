#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE


#include "Sprite.h"
#include "TileMap.h"
#include "Rope.h"



// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.

class Scene;

class Player
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, Scene &scenepar);
	void update(int deltaTime);
	void render();
	
	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);
	bool lastLeft = false;
	int fallpixels = 0;
	glm::ivec2 getPosition();
	
private:
	bool bJumping;
	bool onLadder = false;
	
	glm::ivec2 tileMapDispl, posPlayer;
	int jumpAngle, startY;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;
	ShaderProgram *shaderProgram;
	
	float stunTime = 0;
	Scene* scene;
	int framesAir = 2;
	bool space = false;
	


};


#endif // _PLAYER_INCLUDE


