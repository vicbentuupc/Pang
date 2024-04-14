#include "Rope.h"
#include "ShaderProgram.h"
#include <iostream>

void Rope::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, glm::ivec2 posIni, TileMap &map)
{	spritesheet.loadFromFile("images/rope.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(17, 189), glm::vec2(1.f / 70.f, 1.f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(0, 60);
	for (int i = 0; i < 70; i++) sprite->addKeyframe(0, glm::vec2(i/70.f, 0));

	Ini = posIni;
	sprite->changeAnimation(0);
	posRope = posIni;
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posRope.x), float(tileMapDispl.y + posRope.y)));
	height = 34;
	this->map = &map;
}


void Rope::render()
{
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posRope.x -8), float(tileMapDispl.y + posRope.y - 189)));
	sprite->render();
}

bool Rope::update(int deltaTime)
{
	sprite->update(deltaTime);
	height += 2;
	++animStep;
	if (animStep >= 4) {
		animStep = 0;
		++height;
	}
	if (map->collisionMoveUpRope(glm::ivec2(posRope.x, posRope.y - height), glm::ivec2(9, height))) return false;
	return true;
	
}

int Rope::getyini() {

	return Ini.y+34;
}

int Rope::getyact() {

	return posRope.y - height;
}

int Rope::getx() {

	return posRope.x;
}