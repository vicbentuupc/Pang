
#include <cmath>
#include <iostream>
#include <vector>
#include <GL/glew.h>
#include "powerup.h"
#include "Game.h"
#include "Sprite.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4





void PowerUp::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, int compow, int index)
{
	in = index;
	cpu = compow;
	tileMapDispl = tileMapPos;

	if (cpu == 0) {
		


		spritesheet.loadFromFile("images/powerup" + std::to_string(in) + ".png", TEXTURE_PIXEL_FORMAT_RGBA);
		double aux = 1;
		if (in == 1) aux = 1.f / 3.f;

		sprite = Sprite::createSprite(glm::ivec2(CHAR_WIDTH, CHAR_HEIGHT), glm::vec2(aux, 1), &spritesheet, &shaderProgram);

		if (in != 1) {
			sprite->setNumberAnimations(1);
			sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
		}
		else {
			sprite->setNumberAnimations(1);
			sprite->setAnimationSpeed(0, 7);
			sprite->addKeyframe(0, glm::vec2(0.0f, 0.0f));
			sprite->addKeyframe(0, glm::vec2(1.f / 3.f, 0.0));
			sprite->addKeyframe(0, glm::vec2(2.f / 3.f, 0.0f));
			sprite->changeAnimation(0);
			sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
		}
	}

	else {
		spritesheet.loadFromFile("images/comida" + std::to_string(in) + ".png", TEXTURE_PIXEL_FORMAT_RGBA);
		double aux = 1;
		sprite = Sprite::createSprite(glm::ivec2(CHAR_WIDTH*2, CHAR_HEIGHT*2), glm::vec2(aux, 1), &spritesheet, &shaderProgram);
		sprite->setNumberAnimations(1);
		sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));

	}

}

void PowerUp::update(int deltaTime)
{
	if (cpu == 0) {

		if (posPlayer.y < 184) posPlayer.y += 1;
		if (map->collisionPUDown(posPlayer, glm::ivec2(CHAR_WIDTH, CHAR_WIDTH), &posPlayer.y))
		{
			posPlayer.y -= 1;

		}
	}
	else {

		if (posPlayer.y < 168) posPlayer.y += 1; 
		if (map->collisionPUDown(posPlayer, glm::ivec2(CHAR_WIDTH*2, CHAR_WIDTH*2), &posPlayer.y))
		{
			posPlayer.y -= 1;

		}
	}
	
	sprite->update(deltaTime);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	
}

void PowerUp::render()
{
	sprite->render();
}

void PowerUp::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void PowerUp::setPosition(const glm::vec2& pos)
{
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));

}

glm::ivec2 PowerUp::getpos() {
	return posPlayer;

}


