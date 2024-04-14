#ifndef _TILE_MAP_INCLUDE
#define _TILE_MAP_INCLUDE


#include <glm/glm.hpp>
#include "Texture.h"
#include "ShaderProgram.h"



// Class Tilemap is capable of loading a tile map from a text file in a very
// simple format (see level01.txt for an example). With this information
// it builds a single VBO that contains all tiles. As a result the render
// method draws the whole map independently of what is visible.

class Scene;

class TileMap
{

private:
	TileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program, Scene &scene);

public:
	// Tile maps can only be created inside an OpenGL context
	static TileMap *createTileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program, Scene &scene);

	~TileMap();

	void render() const;
	void free();
	
	int getTileSize() const { return tileSize; }

	bool checkLadders(int x, int y) const;
	bool checkLadderTop(int x, int y) const;
	bool checkLadderGround(int x, int y) const;

	bool checkGround(int x, int y);

	bool collisionMoveLeft(const glm::ivec2 &pos, const glm::ivec2 &size) const;
	bool collisionMoveRight(const glm::ivec2 &pos, const glm::ivec2 &size) const;
	bool collisionMoveDown(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY, int mult) const;
	bool collisionMoveUpRope(const glm::ivec2 &pos, const glm::ivec2 &size) const;

	bool collisionBallDown(const glm::ivec2& pos, const glm::ivec2& size, int* posY, int mult, int lvl, int radio) const;
	void destroyGround(int y, int x) const;
	bool collisionBallLeft(const glm::ivec2& pos, const glm::ivec2& size, int radio) const;
	bool collisionBallRight(const glm::ivec2& pos, const glm::ivec2& size, int radio) const;
	bool collisionBallUp(const glm::ivec2& pos, const glm::ivec2& size, int* posY, int mult, int radio) const;
	int* map;

	bool collisionPUDown(const glm::ivec2& pos, const glm::ivec2& size, int* posY) const;
	
private:
	bool loadLevel(const string &levelFile);
	void prepareArrays(const glm::vec2 &minCoords, ShaderProgram &program);
	bool circleRect(int cx, int cy, float radius, float rx, float ry, float rw, float rh)const;

private:
	GLuint vao;
	GLuint vbo;
	GLint posLocation, texCoordLocation;
	int nTiles;
	glm::ivec2 position, mapSize, tilesheetSize;
	int tileSize, blockSize;
	Texture tilesheet;
	glm::vec2 tileTexSize;
	Scene* scene;
	

};


#endif // _TILE_MAP_INCLUDE


