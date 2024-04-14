#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "TileMap.h"
#include "Scene.h"


using namespace std;

bool TileMap::circleRect(int cx, int cy, float radius, float rx, float ry, float rw, float rh) const{

	// temporary variables to set edges for testing
	float testX = cx;
	float testY = cy;

	// which edge is closest?
	if (cx < rx)         testX = rx;      // test left edge
	else if (cx > rx + rw) testX = rx + rw;   // right edge
	if (cy < ry)         testY = ry;      // top edge
	else if (cy > ry + rh) testY = ry + rh;   // bottom edge

	// get distance from closest edges
	float distX = cx - testX;
	float distY = cy - testY;
	float distance = sqrt((distX * distX) + (distY * distY));

	// if the distance is less than the radius, collision!
	if (distance <= radius) {
		return true;
	}
	return false;
}

TileMap *TileMap::createTileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program, Scene &scene)
{
	TileMap *map = new TileMap(levelFile, minCoords, program, scene);
	
	return map;
}


TileMap::TileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program, Scene &scene)
{
	this->scene = &scene;
	loadLevel(levelFile);
	prepareArrays(minCoords, program);
}

TileMap::~TileMap()
{
	if(map != NULL)
		delete map;
}


void TileMap::render() const
{
	glEnable(GL_TEXTURE_2D);
	tilesheet.use();
	glBindVertexArray(vao);
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(texCoordLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6 * nTiles);
	glDisable(GL_TEXTURE_2D);
}

void TileMap::free()
{
	glDeleteBuffers(1, &vbo);
}

bool TileMap::loadLevel(const string &levelFile)
{
	ifstream fin;
	string line, tilesheetFile;
	stringstream sstream;
	char tile;
	
	fin.open(levelFile.c_str());
	if(!fin.is_open())
		return false;
	getline(fin, line);
	if(line.compare(0, 7, "TILEMAP") != 0)
		return false;
	getline(fin, line);
	sstream.str(line);
	sstream >> mapSize.x >> mapSize.y;
	getline(fin, line);
	sstream.str(line);
	sstream >> tileSize >> blockSize;
	getline(fin, line);
	sstream.str(line);
	sstream >> tilesheetFile;
	tilesheet.loadFromFile(tilesheetFile, TEXTURE_PIXEL_FORMAT_RGBA);
	tilesheet.setWrapS(GL_CLAMP_TO_EDGE);
	tilesheet.setWrapT(GL_CLAMP_TO_EDGE);
	tilesheet.setMinFilter(GL_NEAREST);
	tilesheet.setMagFilter(GL_NEAREST);
	getline(fin, line);
	sstream.str(line);
	sstream >> tilesheetSize.x >> tilesheetSize.y;
	tileTexSize = glm::vec2(1.f / tilesheetSize.x, 1.f / tilesheetSize.y);
	
	map = new int[mapSize.x * mapSize.y];
	for(int j=0; j<mapSize.y; j++)
	{
		for(int i=0; i<mapSize.x; i++)
		{
			fin.get(tile);
			if(tile == ' ')
				map[j*mapSize.x+i] = 0;
			else if(tile >= 'a' && tile <= 'z')
				map[j * mapSize.x + i] = tile - int('a') + 10;
			else
				map[j*mapSize.x+i] = tile - int('0');
		}
		fin.get(tile);
#ifndef _WIN32
		fin.get(tile);
#endif
	}

	fin.close();
	return true;
}

void TileMap::prepareArrays(const glm::vec2 &minCoords, ShaderProgram &program)
{
	int tile;
	glm::vec2 posTile, texCoordTile[2], halfTexel;
	vector<float> vertices;
	
	nTiles = 0;
	halfTexel = glm::vec2(0.5f / tilesheet.width(), 0.5f / tilesheet.height());
	for(int j=0; j<mapSize.y; j++)
	{
		for(int i=0; i<mapSize.x; i++)
		{
			tile = map[j * mapSize.x + i];
			if(tile != 0)
			{
				// Non-empty tile
				nTiles++;
				posTile = glm::vec2(minCoords.x + i * tileSize, minCoords.y + j * tileSize);
				texCoordTile[0] = glm::vec2(float((tile-1)%tilesheetSize.x) / tilesheetSize.x, float((tile-1)/tilesheetSize.x) / tilesheetSize.y);
				texCoordTile[1] = texCoordTile[0] + tileTexSize;
				//texCoordTile[0] += halfTexel;
				texCoordTile[1] -= halfTexel;
				// First triangle
				vertices.push_back(posTile.x); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
				// Second triangle
				vertices.push_back(posTile.x); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
				vertices.push_back(posTile.x); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[1].y);
			}
		}
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 24 * nTiles * sizeof(float), &vertices[0], GL_STATIC_DRAW);
	posLocation = program.bindVertexAttribute("position", 2, 4*sizeof(float), 0);
	texCoordLocation = program.bindVertexAttribute("texCoord", 2, 4*sizeof(float), (void *)(2*sizeof(float)));
}

// Collision tests for axis aligned bounding boxes.
// Method collisionMoveDown also corrects Y coordinate if the box is
// already intersecting a tile below.

bool TileMap::checkLadders(int x, int y) const
{
	int value = map[y * mapSize.x + x];
	if (value == 21 || value == 22 || value == 23 || value == 24 || value == 25 || value == 26)
		return true;
	return false;
}

bool TileMap::checkLadderGround(int x, int y) const
{
	int value = map[y * mapSize.x + x];
	if (value == 21 || value == 22 || value == 23 || value == 0)
		return true;
	return false;
}

bool TileMap::checkLadderTop(int x, int y) const
{
	int value = map[y * mapSize.x + x];
	if (value == 24 || value == 25 || value == 26)
		return true;
	return false;
}

bool TileMap::checkGround(int x, int y)
{
	int value = map[y * mapSize.x + x];
	if (value != 0 && !this->checkLadders(x,y))
		return true;
	return false;
}

bool TileMap::collisionMoveLeft(const glm::ivec2 &pos, const glm::ivec2 &size) const
{
	int x, y0, y1;
	
	x = (pos.x + 7) / tileSize;
	y0 = pos.y / tileSize;
	y1 = (pos.y + size.y - 1) / tileSize;
	for(int y=y0; y<=y1; y++)
	{
		if(map[y*mapSize.x+x] != 0 && !checkLadders(x,y))
			return true;
	}
	
	return false;
}

bool TileMap::collisionMoveRight(const glm::ivec2 &pos, const glm::ivec2 &size) const
{
	int x, y0, y1;
	
	x = (pos.x + size.x - 7) / tileSize;
	y0 = pos.y / tileSize;
	y1 = (pos.y + size.y - 1) / tileSize;
	for(int y=y0; y<=y1; y++)
	{
		if(map[y*mapSize.x+x] != 0 && !checkLadders(x,y))
			return true;
	}
	
	return false;
}


bool TileMap::collisionMoveDown(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY, int mult) const
{
	int x0, x1, y;
	
	x0 = (pos.x+7) / tileSize;
	x1 = (pos.x + size.x - 7) / tileSize;
	y = (pos.y + size.y - 1) / tileSize;
	for(int x=x0; x<=x1; x++)
	{
		if(map[y*mapSize.x+x] != 0)
		{
			if(*posY - tileSize * y + size.y <= 4 * mult)
			{
				*posY = tileSize * y - size.y;
				return true;
			}
		}
	}
	
	return false;
}

bool TileMap::collisionPUDown(const glm::ivec2& pos, const glm::ivec2& size, int* posY) const
{
	int x0, x1, y;

	x0 = (pos.x + 7) / tileSize;
	x1 = (pos.x + size.x - 7) / tileSize;
	y = (pos.y + size.y - 1) / tileSize;
	for (int x = x0; x <= x1; x++)
	{
		if (map[y * mapSize.x + x] != 0 && !checkLadderTop(x, y))
		{
			if (*posY - tileSize * y + size.y <= 4)
			{
				return true;
			}
		}
	}

	return false;
}

bool TileMap::collisionMoveUpRope(const glm::ivec2& pos, const glm::ivec2& size) const
{
	int x0, x1, y;
	
	x0 = (pos.x-4) / tileSize;
	x1 = (pos.x + 4) / tileSize;
	y = pos.y / tileSize;
	for (int x = x0; x <= x1; x++)
	{
		if (map[y * mapSize.x + x] != 0 && !(map[y * mapSize.x + x] == 24 || map[y * mapSize.x + x] == 25 || map[y * mapSize.x + x] == 26)) {

			if (map[y * mapSize.x + x] == 13 || map[y * mapSize.x + x] == 14 || map[y * mapSize.x + x] == 15 || map[y * mapSize.x + x] == 9 || map[y * mapSize.x + x] == 10 || map[y * mapSize.x + x] == 11 || map[y * mapSize.x + x] == 12) {
				destroyGround(y, x);
			}
			return true;
		}
	}
	
	return false;
}

bool TileMap::collisionBallLeft(const glm::ivec2& pos, const glm::ivec2& size, int radio) const
{
	int x, y0, y1;

	x = pos.x / tileSize;
	y0 = pos.y / tileSize;
	y1 = (pos.y + size.y - 1) / tileSize;
	for (int y = y0; y <= y1; y++)
	{

		if (map[y * mapSize.x + x] != 0 && !checkLadderTop(x, y)) {
			if (radio == 7) return true;
			if (circleRect(pos.x + size.x/ 2, pos.y + radio / 2, float(radio/2), float(x * 8), float(y * 8), float(8), float(8))) {

				return true;
			}
			
		}
	}

	return false;
}


bool TileMap::collisionBallRight(const glm::ivec2& pos, const glm::ivec2& size, int radio) const
{
	int x, y0, y1;

	x = (pos.x + size.x - 1) / tileSize;
	y0 = pos.y / tileSize;
	y1 = (pos.y + size.y - 1) / tileSize;
	for (int y = y0; y <= y1; y++)
	{
		if (map[y * mapSize.x + x] != 0 && !checkLadderTop(x, y)) {

			if (radio == 7) return true;
			if (circleRect(pos.x + size.x / 2, pos.y + size.y / 2, float(radio/2), float(x * 8), float(y * 8), float(8), float(8))) {
				return true;
			}
			
		}
			
	}

	return false;
}
bool TileMap::collisionBallUp(const glm::ivec2& pos, const glm::ivec2& size, int* posY, int mult,int radio) const
{
	int x0, x1, y;

	x0 = pos.x / tileSize;
	x1 = (pos.x + size.x - 1) / tileSize;
	y = pos.y / tileSize; // Cambio en cómo se calcula 'y'
	for (int x = x0; x <= x1; x++)
	{
		if (map[y * mapSize.x + x] != 0 && !checkLadderTop(x, y))
		{
			if (*posY - (tileSize * (y + 1)) >= -4 * mult) // Cambio en la condición de colisión
			{
				if (radio == 7) return true;
				if (circleRect(pos.x + size.x/2, pos.y + radio/2, float(radio/2), float(x * 8), float(y * 8), float(8), float(8))){
					*posY = tileSize * (y + 1); // Cambio en la asignación de la posición 'y'
					return true;
				}
				
				
			}
		}
	}

	return false;
}

bool TileMap::collisionBallDown(const glm::ivec2& pos, const glm::ivec2& size, int* posY, int mult, int lvl,int radio) const
{
	int x0, x1, y;
	if (lvl == 4) {
		x0 = (pos.x-1) / tileSize;
		x1 = (pos.x + size.x-7) / tileSize;
		y = (pos.y + size.y-1) / tileSize;
		if (x0 == 0) x0 = 1;

	}

	else {
		x0 = (pos.x + 7) / tileSize;
		x1 = (pos.x + size.x - 7) / tileSize;
		y = (pos.y + size.y - 1) / tileSize;
	}
	
	for (int x = x0; x <= x1; x++)
	{
		if (map[y * mapSize.x + x] != 0 && !checkLadderTop(x,y))
		{
			if (*posY - tileSize * y + size.y <= 4* mult)
			{
				if (radio == 7) return true;
				if (circleRect(pos.x + size.x/2, pos.y + radio/2, float(radio/2), float(x * 8), float(y * 8), float(8), float(8))) {
					*posY = tileSize * y - size.y;
					return true;
				}
				
			}
		}
	}

	return false;
}


void TileMap::destroyGround(int y, int x) const {
	if (map[y * mapSize.x + x] == 13) map[y * mapSize.x + x] = map[y * mapSize.x + x+1] = map[y * mapSize.x + x+2] = 0;
	if (map[y * mapSize.x + x] == 14) map[y * mapSize.x + x-1] = map[y * mapSize.x + x] = map[y * mapSize.x + x+1] = 0;
	if (map[y * mapSize.x + x] == 15) map[y * mapSize.x + x-2] = map[y * mapSize.x + x-1] = map[y * mapSize.x + x] = 0;
	if (map[y * mapSize.x + x] == 9) map[y * mapSize.x + x] = map[y * mapSize.x + x+1] = map[y * mapSize.x + x+2] = map[y * mapSize.x + x+3] = 0;
	if (map[y * mapSize.x + x] == 10) map[y * mapSize.x + x-1] = map[y * mapSize.x + x] = map[y * mapSize.x + x+1] = map[y * mapSize.x + x+2] = 0;
	if (map[y * mapSize.x + x] == 11) map[y * mapSize.x + x-2] = map[y * mapSize.x + x-1] = map[y * mapSize.x + x] = map[y * mapSize.x + x+1] = 0;
	if (map[y * mapSize.x + x] == 12) map[y * mapSize.x + x-3] = map[y * mapSize.x + x-2] = map[y * mapSize.x + x-1] = map[y * mapSize.x + x] = 0;
	scene->destroyPlatform(x,y);

}