#ifndef _BUBBLE_INCLUDE
#define _BUBBLE_INCLUDE


#include "Sprite.h"
#include "TileMap.h"





class Bubble
{

public:
	

	void initburbuja1hw();
	void initburbuja1();
	void initburbuja2hw();
	void initburbuja2();
	void initburbuja3hw();
	void initburbuja3();
	void initburbuja4hw();
	void initburbuja4();
	void initburbuja5hw(int a);
	void initburbuja5();
	int getmode();

	int getw(int n);
	int geth(int n);
	glm::ivec2 getpos();
	glm::ivec2 getpostot();
	glm::ivec2 getpostot2();
	
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram,int mode1, bool der, bool abj, bool prab1, bool prarr1, int ymax, int clevel);
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);
	bool stop = false;
	Sprite* sprite;

private:

	int mode;
	int possuelo;
	bool derecha, abajo,prab,prarr;
	bool firstcol = false;
	int level;
	int CHAR_HEIGHT;
	int CHAR_WIDTH;
	int altmax;
	glm::ivec2 tileMapDispl, posPlayer;
	std::vector<float> pos;
	int cant;
	int ipos = 1;
	int ymax;
	Texture spritesheet;
	
	TileMap* map;

};


#endif // _BUBBLE_INCLUDE

