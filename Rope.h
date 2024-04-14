
#ifndef _ROPE_INCLUDE
#define _ROPE_INCLUDE
#include "Sprite.h"
#include "TileMap.h"

class Rope
{
	public:
		void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, glm::ivec2 posIni, TileMap &map);

		void render();

		bool update(int deltaTime);
		Sprite* sprite;

		int getyini();
		int getyact();
		int getx();

	private:
		Texture spritesheet;
		TileMap *map;
		glm::ivec2 Ini;
		int height;
		int animStep = 0;
		
		glm::ivec2 tileMapDispl, posRope;
};















#endif // _ROPE_INCLUDE