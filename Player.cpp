#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Player.h"
#include "Game.h"
#include "Scene.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4
#define CHAR_WIDTH 32


enum PlayerAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT, STAIRS, CLIMB, ENDCLIMB, SHOOTLEFT, SHOOTRIGHT
};

enum Weapons
{
	BASIC
};

int fallSteps[] = { 1, 0,1,0, 1, 1, 2, 1, 2, 2, 2, 3, 2, 2, 3, 4, 3, 3, 4, 4, 5, 3, 5, 5, 6, 7, 6, 7 ,7 ,7, 7, 7, 7 ,7, 7, 7, 7 ,7, 7, 7,7 ,7, 7, 7,7 ,7, 7, 7,7 ,7, 7, 7};


void Player::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, Scene &scenepar)
{
	this->scene = &scenepar;
	this->shaderProgram = &shaderProgram;
	int mult = 2;
	bJumping = false;
	spritesheet.loadFromFile("images/pangplayerprova.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(CHAR_WIDTH, CHAR_WIDTH), glm::vec2(0.1*mult, 0.1 * mult), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(9);

	sprite->setAnimationSpeed(STAND_LEFT, 8);
	sprite->addKeyframe(STAND_LEFT, glm::vec2(0.1f * mult, 0.2f * mult));

	sprite->setAnimationSpeed(STAND_RIGHT, 8);
	sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.f * mult, 0.2f * mult));

	sprite->setAnimationSpeed(MOVE_LEFT, 15);
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f * mult, 0.1f * mult));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.1f * mult, 0.1f * mult));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.2f * mult, 0.1f * mult));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.3f * mult, 0.1f * mult));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.4f * mult, 0.1f * mult));




	sprite->setAnimationSpeed(MOVE_RIGHT, 15);
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.f * mult, 0.f * mult));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.1f * mult, 0.f * mult));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.2f * mult, 0.f * mult));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.3f * mult, 0.f * mult));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.4f * mult, 0.f * mult));

	sprite->setAnimationSpeed(STAIRS, 15);
	sprite->addKeyframe(STAIRS, glm::vec2(0.2f * mult, 0.2f * mult));
	sprite->addKeyframe(STAIRS, glm::vec2(0.4f * mult, 0.2f * mult));
	sprite->addKeyframe(STAIRS, glm::vec2(0.3f * mult, 0.2f * mult));
	sprite->addKeyframe(STAIRS, glm::vec2(0.4f * mult, 0.2f * mult));

	sprite->setAnimationSpeed(CLIMB, 15);
	sprite->addKeyframe(CLIMB, glm::vec2(0.f * mult, 0.3f * mult));

	sprite->setAnimationSpeed(ENDCLIMB, 15);
	sprite->addKeyframe(ENDCLIMB, glm::vec2(0.1f * mult, 0.3f * mult));

	sprite->setAnimationSpeed(SHOOTRIGHT, 20);
	sprite->addKeyframe(SHOOTRIGHT, glm::vec2(0.1f * mult, 0.3f * mult));

	sprite->setAnimationSpeed(SHOOTLEFT, 20);
	sprite->addKeyframe(SHOOTLEFT, glm::vec2(0.2f * mult, 0.3f * mult));







	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));

}

void Player::update(int deltaTime)
{
	
	sprite->update(deltaTime);
	
	if (stunTime > 0) {
		if (sprite->animation() == ENDCLIMB) {
			if (onLadder) sprite->changeAnimation(STAIRS);
			else if (lastLeft) sprite->changeAnimation(STAND_LEFT);
			else sprite->changeAnimation(STAND_RIGHT);
		}
		stunTime -= deltaTime;
		if (stunTime <= 0 && sprite->animation() == SHOOTLEFT) sprite->changeAnimation(STAND_LEFT);
		if (stunTime <= 0 && sprite->animation() == SHOOTRIGHT) sprite->changeAnimation(STAND_RIGHT);
		return;
	}

	if (!onLadder) {
		if (Game::instance().getKey(GLFW_KEY_LEFT))
		{
			if (sprite->animation() != MOVE_LEFT) {
				sprite->changeAnimation(MOVE_LEFT);
				lastLeft = true;
			}
			posPlayer.x -= 2 * CHAR_WIDTH / 32;
			if (map->collisionMoveLeft(posPlayer, glm::ivec2(CHAR_WIDTH, CHAR_WIDTH)))
			{
				posPlayer.x += 2 * CHAR_WIDTH / 32;
				//sprite->changeAnimation(STAND_LEFT);
			}
		}
		else if (Game::instance().getKey(GLFW_KEY_RIGHT))
		{
			if (sprite->animation() != MOVE_RIGHT) {
				sprite->changeAnimation(MOVE_RIGHT);
				lastLeft = false;
			}

			posPlayer.x += 2 * CHAR_WIDTH / 32;
			if (map->collisionMoveRight(posPlayer, glm::ivec2(CHAR_WIDTH, CHAR_WIDTH)))
			{
				posPlayer.x -= 2 * CHAR_WIDTH / 32;
				//sprite->changeAnimation(STAND_RIGHT);
			}
		}
		else
		{
			if (lastLeft)
				sprite->changeAnimation(STAND_LEFT);
			else
				sprite->changeAnimation(STAND_RIGHT);
		}
		if (Game::instance().getKey(GLFW_KEY_UP))
		{
			glm::ivec2 centerPlayer = glm::ivec2((posPlayer.x + CHAR_WIDTH / 2) / map->getTileSize(), (posPlayer.y + CHAR_WIDTH / 2)/map->getTileSize());
			if (map->checkLadders(centerPlayer.x, centerPlayer.y-1)) {
				sprite->changeAnimation(STAIRS);
				onLadder = true;
				posPlayer.y -= 4 * CHAR_WIDTH / 32;
				while (map->checkLadders((posPlayer.x + 4) / map->getTileSize(), centerPlayer.y - 1)) {
					posPlayer.x -= 2 * CHAR_WIDTH / 32;
				}
				while (map->checkLadders((posPlayer.x + CHAR_WIDTH-4)/map->getTileSize(), centerPlayer.y-1)) {
					posPlayer.x += 2 * CHAR_WIDTH / 32;
				}
				
			}
		}
		else if (Game::instance().getKey(GLFW_KEY_DOWN))
		{
			glm::ivec2 centerPlayer = glm::ivec2((posPlayer.x + CHAR_WIDTH / 2) / map->getTileSize(), (posPlayer.y + CHAR_WIDTH / 2) / map->getTileSize());
			if (map->checkLadders(centerPlayer.x, centerPlayer.y + 2)) {
				sprite->changeAnimation(CLIMB);
				stunTime = 75;
				onLadder = true;
				posPlayer.y += 10 * CHAR_WIDTH / 32;
				while (map->checkLadders((posPlayer.x + 4) / map->getTileSize(), centerPlayer.y + 2)) {
					posPlayer.x -= 2 * CHAR_WIDTH / 32;
				}
				while (map->checkLadders((posPlayer.x + CHAR_WIDTH - 4) / map->getTileSize(), centerPlayer.y + 2)) {
					posPlayer.x += 2 * CHAR_WIDTH / 32;
				}
				
			}
		}

		//posPlayer.y += 2 * CHAR_WIDTH / 32;
		//map->collisionMoveDown(posPlayer, glm::ivec2(CHAR_WIDTH, CHAR_WIDTH), &posPlayer.y, CHAR_WIDTH / 32);
		++fallpixels;
		for (int i = 0; i < fallSteps[fallpixels] * CHAR_WIDTH / 32; ++i) {
			posPlayer.y += CHAR_WIDTH / 32;
			if (map->collisionMoveDown(posPlayer, glm::ivec2(CHAR_WIDTH, CHAR_WIDTH), &posPlayer.y, CHAR_WIDTH / 32)) {
				fallpixels = 0;
				break;
			}
		}
		


	}
	else {
		glm::ivec2 centerPlayer = glm::ivec2((posPlayer.x + CHAR_WIDTH / 2) / map->getTileSize(), (posPlayer.y + CHAR_WIDTH / 2) / map->getTileSize());
		if (Game::instance().getKey(GLFW_KEY_UP)) {
			posPlayer.y -= 2 * CHAR_WIDTH / 32;
			
			if (!map->checkLadders(centerPlayer.x, centerPlayer.y + 1) && !map->checkLadders(centerPlayer.x, centerPlayer.y)) {
				onLadder = false;
				sprite->changeAnimation(CLIMB);
				posPlayer.y -= 4 * CHAR_WIDTH / 32;
				stunTime = 150;
			}
		}
		else if (Game::instance().getKey(GLFW_KEY_DOWN)) {
			if (!map->checkLadders(centerPlayer.x, centerPlayer.y) && !map->checkLadders(centerPlayer.x,centerPlayer.y+1) || map->checkGround(centerPlayer.x, centerPlayer.y+2)) {
				onLadder = false;
				if (lastLeft) sprite->changeAnimation(STAND_LEFT);
				else sprite->changeAnimation(STAND_RIGHT);
			}
			else posPlayer.y += 2 * CHAR_WIDTH / 32;
			
		}

		
		
	}
	if (Game::instance().getKey(GLFW_KEY_SPACE)) {
		if (!space) {
			space = true;
			if (!scene->createRope(glm::ivec2(posPlayer.x + (CHAR_WIDTH / 2), posPlayer.y + CHAR_WIDTH))) return;
			stunTime = 100;
			if (!onLadder) {
				if (lastLeft) sprite->changeAnimation(SHOOTLEFT);
				else sprite->changeAnimation(SHOOTRIGHT);
			}
		}
	}
	else space = false;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Player::render()
{
	sprite->render();
}

void Player::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Player::setPosition(const glm::vec2 &pos)
{
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

glm::ivec2 Player::getPosition()
{
	return posPlayer;
}