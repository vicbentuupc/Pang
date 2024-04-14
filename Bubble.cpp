#include <cmath>
#include <iostream>
#include <vector>
#include <GL/glew.h>
#include "Bubble.h"
#include "Game.h"
#include "Sprite.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4





void Bubble::initburbuja1hw() {

	CHAR_HEIGHT = 40;
	CHAR_WIDTH = 48;
	altmax = 24;
	possuelo = 59;
	cant = 1;
}

void Bubble::initburbuja2hw() {

	CHAR_HEIGHT = 26;
	CHAR_WIDTH = 32;
	altmax = 68;
	possuelo = 55;
	cant = 1;
}

void Bubble::initburbuja3hw() {

	CHAR_HEIGHT = 14;
	CHAR_WIDTH = 16;
	altmax = 104;
	possuelo = 50;
	cant = 1;
}

void Bubble::initburbuja4hw() {

	CHAR_HEIGHT = 7;
	CHAR_WIDTH = 8;
	altmax = 152;
	possuelo = 39;
	cant = 1;
}

void Bubble::initburbuja5hw(int a) {

	CHAR_HEIGHT = 32;
	CHAR_WIDTH = 48;
	altmax = a;
	possuelo = 0;
	cant = 2;
	if (firstcol) cant = 1;
	

}

void Bubble::initburbuja1() {
	for (int i = 0; i < pos.size(); ++i) { pos.pop_back(); }
	for (int i = 0; i < 90; ++i) {
		pos.push_back(-(540 * 4 * i * 4 * i / 86024) + 540);
	}

	for (int i = 89; i > 1; --i) {
		pos[i] = pos[i - 1] - pos[i];
		
	}
	
	pos[1] = 1;

	for (int i = 0; i < 90; ++i) {
		if (pos[i] == 0) pos[i] = 1;
		

	}

	



	int pos2;

	int c = 0;
	for (int i = 1; i < 90; ++i) {
		pos2 = pos[i];
		if (pos2 >= 4 ) {

			if (pos2 % 4 != 0) {
				c += (pos2 % 4);
				pos2 -= (pos2 % 4);
				pos2 /= 4;
				if (c >= 4) {
					c -= 4;
					pos2 += 1;
				}


			}
			else pos2 /= 4;
		}

		else if (pos2 < 4) {
			if (pos2 % 4 != 0) {
				c += (pos2 % 4);
				pos2 -= (pos2 % 4);
				if (c >= 4) {
					c -= 4;
					pos2 += 1;
				}


			}
		}
		
		pos[i] = pos2;


	}
	for (int i = 0; i < 90; ++i) {
		if (pos[i] != 0) pos[i] *= 1.3f;

	}
	


}
void Bubble::initburbuja2() {

	for (int i = 0; i < pos.size(); ++i) { pos.pop_back(); }
	
	

	for (int i = 0; i < 90; ++i) {
		
		pos.push_back( - (445  * 4 * i * 4 * i /50176) + 445);
	}
	


	for (int i = 89; i > 1; --i) {
		pos[i] = pos[i - 1] - pos[i];
		
	}

	pos[1] = 1;

	for (int i = 0; i < 90; ++i) {
		if (pos[i] == 0) pos[i] = 1;
		
	}

	int pos2;

	int c = 0;
	for (int i = 1; i < 90; ++i) {
		pos2 = pos[i];
		if (pos2 >= 4) {

			if (pos2 % 4 != 0) {
				c += (pos2 % 4);
				pos2 -= (pos2 % 4);
				pos2 /= 4;
				if (c >= 4) {
					c -= 4;
					pos2 += 1;
				}


			}
			else pos2 /= 4;
		}

		else if (pos2 < 4) {
			if (pos2 % 4 != 0) {
				c += (pos2 % 4);
				pos2 -= (pos2 % 4);
				if (c >= 4) {
					c -= 4;
					pos2 += 1;
				}


			}
		}

		pos[i] = pos2;


	}

	
	
	

}

void Bubble::initburbuja3() {
	for (int i = 0; i < pos.size(); ++i) { pos.pop_back(); }
	
	for (int i = 0; i < 110; ++i) {
		
		pos.push_back(-(341 * 4 * i * 4 * i / 40804) + 341);
	}

	for (int i = 109; i > 1; --i) {
		pos[i] = pos[i - 1] - pos[i];
		
	}

	pos[1] = 1;
	for (int i = 0; i < 110; ++i) {
		
		if (pos[i] == 0) pos[i] = 1;
		
	}
	int pos2;

	int c = 0;
	for (int i = 1; i < 110; ++i) {
		pos2 = pos[i];
		if (pos2 >= 4) {

			if (pos2 % 4 != 0) {
				c += (pos2 % 4);
				pos2 -= (pos2 % 4);
				pos2 /= 4;
				if (c >= 4) {
					c -= 4;
					pos2 += 1;
				}


			}
			else pos2 /= 4;
		}

		else if (pos2 < 4) {
			if (pos2 % 4 != 0) {
				c += (pos2 % 4);
				pos2 -= (pos2 % 4);
				if (c >= 4) {
					c -= 4;
					pos2 += 1;
				}


			}
		}

		pos[i] = pos2;


	}
	





}
void Bubble::initburbuja4() {
	
	for (int i = 0; i < pos.size(); ++i) { pos.pop_back(); }
	

	for (int i = 0; i < 191; ++i) {
		
		pos.push_back(-(166  * 4 * i * 4 * i / 24649) + 166);
	}

	for (int i = 190; i > 1; --i) {
		pos[i] = pos[i - 1] - pos[i];
	}

	pos[1] = 1;
	for (int i = 0; i < 191; ++i) {

		if (pos[i] == 0) pos[i] = 1;
	}

	int pos2;

	int c = 0;
	for (int i = 1; i < 191; ++i) {
		pos2 = pos[i];
		if (pos2 >= 4) {

			if (pos2 % 4 != 0) {
				c += (pos2 % 4);
				pos2 -= (pos2 % 4);
				pos2 /= 4;
				if (c >= 4) {
					c -= 4;
					pos2 += 1;
				}


			}
			else pos2 /= 4;
		}

		else if (pos2 < 4) {
			if (pos2 % 4 != 0) {
				c += (pos2 % 4);
				pos2 -= (pos2 % 4);
				if (c >= 4) {
					c -= 4;
					pos2 += 1;
				}


			}
		}

		pos[i] = pos2;


	}

	

	

}

void Bubble::initburbuja5() {
	if (!firstcol) {
		for (int i = 0; i < pos.size(); ++i) { pos.pop_back(); }
		for (int i = 0; i < 1000; ++i) {
			pos.push_back(-(73 * 4 * i * 4 * i / 5329) + 73);
		}
		for (int i = 999; i > 1; --i) {
			pos[i] = pos[i - 1] - pos[i];

		}
	}

	else {
		
		for (int i = 0; i < 1000; ++i) {
			pos[i] /= 1.7;
		}
		
	}


	


	for (int i = 999; i > 1; --i) {
		pos[i] /= 1.5f;

	}



	pos[1] = 1;



}

void Bubble ::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, int mode1, bool der, bool abj, bool prab1,bool prarr1, int ymax1,int clevel)
{
	level = clevel;
	
	prab = prab1;
	prarr = prarr1;
	
	mode = mode1;
	int mult = 1;
	
	derecha = der;
	abajo = abj;
	tileMapDispl = tileMapPos;
	if (mode == 1) {
		initburbuja1hw();
	}
	else if (mode == 2) {
		initburbuja2hw();
	}
	else if (mode == 3) {
		initburbuja3hw();
	}
	else if (mode == 4) {
		initburbuja4hw();
	}
	else if (mode == 5) {
		initburbuja5hw(ymax1);
	}
	if (mode == 1) {
		initburbuja1();
	}
	else if (mode == 2) {
		initburbuja2();
	}
	else if (mode == 3) {
		initburbuja3();
	}
	else if (mode == 4) {
		initburbuja4();
	}
	else if (mode == 5) {
		initburbuja5();
	}

	if (!prarr) {
		ipos = 20; 
		ymax = ymax1;
		for (int i = 1; i <= ipos; ++i) ymax -= pos[ipos];
	}
	else ymax = ymax1;

	if (mode == 5) {
		int a = 70;
		for (int i = 1; a > 0; ++i) {
			a -= pos[i];
			possuelo = i;
		}
		ipos = possuelo;

	}
	else {
		if (ymax > altmax) {

			prab = true;
		}
	}

	if(mode == 5) spritesheet.loadFromFile("images/burbuja" + std::to_string(1) + ".png", TEXTURE_PIXEL_FORMAT_RGBA);
	if (level == 2) {
		spritesheet.loadFromFile("images/burbujaa" + std::to_string(mode) + ".png", TEXTURE_PIXEL_FORMAT_RGBA);
	}
	else spritesheet.loadFromFile("images/burbuja"+ std::to_string(mode) + ".png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(CHAR_WIDTH, CHAR_HEIGHT), glm::vec2(1, 1), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	
}

void Bubble::update(int deltaTime)
{
	if (mode == 5 && ipos == pos.size() - 1) stop = true;
	if (!stop) {
		float xant = posPlayer.x;
		float yant = posPlayer.y;

		if (!abajo) {
			bool act = true;
			if (prab && !prarr && posPlayer.y<altmax) {
				posPlayer.y = altmax;
				prarr = true;
				ipos = 1;
				abajo = true;
				act = false;
				
			}
			if (act) {
				
				for (int i = 0; i < pos[ipos] && !abajo;++i) {
					posPlayer.y -= 1;
					float aux = CHAR_HEIGHT / 8;
					if (posPlayer.y < altmax && prarr) {
						ipos = 1;
						abajo = true;
					}
					if (mode == 4) aux = 1;

					int aux2 = CHAR_HEIGHT;
					if (!firstcol) {
						if (map->collisionBallUp(posPlayer, glm::ivec2(CHAR_WIDTH, aux2), &posPlayer.y, aux, aux2))
						{
							abajo = true;
							posPlayer.y = yant;
							ipos += 2;
							if (mode == 5)
							{
								firstcol = true;
								initburbuja5hw(posPlayer.y - 32);
								initburbuja5();
								int a = 32;
								for (int i = 1; a > 0; ++i) {
									a -= pos[i];
									possuelo = i;
								}
								ipos = possuelo;
							}



						}
					}
				}
				--ipos;
				if (ipos == 0) {
					ipos = 1;
					abajo = true;
				}

			}
		}
		else if (abajo) {


			int aux = CHAR_HEIGHT / 8;
			for (int i = 0; i < pos[ipos] && abajo;++i) {
				posPlayer.y += 1;
				int aux2 = CHAR_HEIGHT;
				if (mode == 4) aux = 1;
				if (!firstcol) {
					if (map->collisionBallDown(posPlayer, glm::ivec2(CHAR_WIDTH, aux2), &posPlayer.y, aux, mode, aux2))
					{

						abajo = false;
						posPlayer.y -= 1;
						ipos -= 2;
						if (posPlayer.y > altmax) {
							prab = true;
							if (!prarr) {

								int a = posPlayer.y - altmax;
								for (int i = 1; a > 0; ++i) {
									a -= pos[i];
									possuelo = i;
								}
								ipos = possuelo +1;

							}

						}
						if (mode == 5)
						{
							firstcol = true;
							initburbuja5hw(posPlayer.y - 32);
							initburbuja5();
							int a = 32;
							for (int i = 1; a > 0; ++i) {
								a -= pos[i];
								possuelo = i;
							}
							ipos = possuelo;
						}
					}
				}
			}
			++ipos;

		}

		if (derecha) {

			int x = posPlayer.x + cant;
			if (!firstcol) {
				if (map->collisionBallRight(glm::vec2(x, yant), glm::ivec2(CHAR_WIDTH, CHAR_HEIGHT), CHAR_HEIGHT))
				{
					x = xant;
					derecha = false;
					if (mode == 5)
					{
						firstcol = true;
						initburbuja5hw(posPlayer.y - 32);
						initburbuja5();
						int a = 32;
						for (int i = 1; a > 0; ++i) {
							a -= pos[i];
							possuelo = i;
						}
						ipos = possuelo;
					}

				}
			}
			if (abajo) abajo = true;
			posPlayer.x = x;
		
		}

		else if (!derecha) {

			int x = posPlayer.x - cant;

			if (!firstcol) {
				if (map->collisionBallLeft(glm::vec2(x, yant), glm::ivec2(CHAR_WIDTH, CHAR_HEIGHT), CHAR_HEIGHT))
				{
					x = xant;
					derecha = true;
					if (mode == 5)
					{
						firstcol = true;
						initburbuja5hw(posPlayer.y - 32);
						initburbuja5();
						int a = 32;
						for (int i = 1; a > 0; ++i) {
							a -= pos[i];
							possuelo = i;
						}
						ipos = possuelo;
					}

				}
			}
			posPlayer.x = x;




		}

		
		//if (suelo) sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + aux4)));
		sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	}
}

void Bubble::render()
{
	sprite->render();
}

void Bubble::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void Bubble::setPosition(const glm::vec2& pos)
{
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	
}

int Bubble::getmode() { return mode; }
int Bubble::getw(int n) {
	if(n == 1) {
		initburbuja1hw();
	}
	else if (n == 2) {
		initburbuja2hw();
	}
	else if (n == 3) {
		initburbuja3hw();
		
	}
	else if (n == 4) {
		initburbuja4hw();
		
	}
	return CHAR_WIDTH;

}
int Bubble::geth(int n) {
	if (n == 1) {
		initburbuja1hw();
	}
	else if (n == 2) {
		initburbuja2hw();
	}
	else if (n == 3) {
		initburbuja3hw();

	}
	else if (n == 4) {
		initburbuja4hw();

	}
	return CHAR_HEIGHT;

}
glm::ivec2 Bubble::getpos() {
	return posPlayer;

}

glm::ivec2 Bubble::getpostot2() {
	if (mode <= 2) return glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y));
	else return glm::vec2(float(tileMapDispl.x  + posPlayer.x), float(tileMapDispl.y  + posPlayer.y));
}

glm::ivec2 Bubble::getpostot() {
	if (mode<=2) return glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y));
	else return glm::vec2(float(tileMapDispl.x - 16 + posPlayer.x), float(tileMapDispl.y - 16 + posPlayer.y));
}
