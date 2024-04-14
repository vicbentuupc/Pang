
#ifndef _PLATFORM_INCLUDE
#define _PLATFORM_INCLUDE

#include "Sprite.h"


class Platform
{
public:
	void init(glm::ivec2 pos, ShaderProgram& shaderProgram, int color, int* map);

	void render();
	void update(int deltaTime);
	int posx, posy;
	Sprite* sprite;

private:
	int color; // 0 = blue, 1 = yellow
	
	
	Texture spritesheet;

};















#endif // _ROPE_INCLUDE