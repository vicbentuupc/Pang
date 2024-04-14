#include "Platform.h"
#include "Game.h"
#include <iostream>

void Platform::init(glm::ivec2 pos, ShaderProgram& shaderProgram, int color, int* map)
{
	this->color = color;
	this->posx = pos.x;
	this->posy = pos.y;

	if (color == 1) { //yellow 4width
		spritesheet.loadFromFile("images/pangblocks.png", TEXTURE_PIXEL_FORMAT_RGBA);
		sprite = Sprite::createSprite(glm::ivec2(8*4, 8), glm::vec2(0.4f, 0.1f), &spritesheet, &shaderProgram);
		sprite->setNumberAnimations(2);

		sprite->setAnimationSpeed(0, 8);
		sprite->addKeyframe(0, glm::vec2(0.0f, 0.4f));

		sprite->setAnimationSpeed(1, 15);
		sprite->addKeyframe(1, glm::vec2(0.0f, 0.6f));
		sprite->addKeyframe(1, glm::vec2(0.4f, 0.6f));
		sprite->addKeyframe(1, glm::vec2(0.0f, 0.7f));
		sprite->addKeyframe(1, glm::vec2(0.4f, 0.7f));
		sprite->addKeyframe(1, glm::vec2(0.4f, 0.7f));
	}
	else { //blue 3width
		spritesheet.loadFromFile("images/pangblocks.png", TEXTURE_PIXEL_FORMAT_RGBA);
		sprite = Sprite::createSprite(glm::ivec2(8*3, 8), glm::vec2(0.3f, 0.1f), &spritesheet, &shaderProgram);
		sprite->setNumberAnimations(2);

		sprite->setAnimationSpeed(0, 8);
		sprite->addKeyframe(0, glm::vec2(0.2f, 0.1f));

		sprite->setAnimationSpeed(1, 15);
		sprite->addKeyframe(1, glm::vec2(0.6f, 0.2f));
		sprite->addKeyframe(1, glm::vec2(0.0f, 0.3f));
		sprite->addKeyframe(1, glm::vec2(0.3f, 0.3f));
		sprite->addKeyframe(1, glm::vec2(0.6f, 0.3f));
		sprite->addKeyframe(1, glm::vec2(0.6f, 0.3f));
	}

	sprite->changeAnimation(0);
	sprite->setPosition(glm::vec2(float(posx), float(posy)));
	


}

void Platform::render()
{
	sprite->render();
}

void Platform::update(int deltaTime)
{
	sprite->update(deltaTime);
}