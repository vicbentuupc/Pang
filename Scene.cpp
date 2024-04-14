#include <iostream>
#include <cmath>
#include <random>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"
#include <windows.h>
#pragma comment(lib, "winmm.lib")

#include <sstream> // For std::ostringstream
#include <iomanip> // For std::setw and std::setfill


#define SCREEN_X 32
#define SCREEN_Y 16

#define INIT_PLAYER_X_TILES 21
#define INIT_PLAYER_Y_TILES 21


Scene::Scene()
{
	map = NULL;
	player = NULL;
}

Scene::~Scene()
{
	if(map != NULL)
		delete map;
	if(player != NULL)
		delete player;
}


void Scene::init()
{
	initShaders();
	projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.f);
	muerto = true;
	
	
	//intro video
	defaultSpritesheet = new Texture();
	defaultSpritesheet->loadFromFile("vid/intro/frame_0040.png", TEXTURE_PIXEL_FORMAT_RGBA);
	defaultSprite = Sprite::createSprite(glm::ivec2(SCREEN_WIDTH, SCREEN_HEIGHT), glm::vec2(1, 1), defaultSpritesheet, &texProgram);
	defaultSprite->setPosition(glm::vec2(0, 0));
	defaultSprite->setNumberAnimations(1);
	defaultSprite->setAnimationSpeed(0, 1);
	defaultSprite->addKeyframe(0, glm::vec2(0, 0));
	defaultSprite->changeAnimation(0);
	const int NUM_FRAMES = 59;

	for (int i = 1; i <= NUM_FRAMES; ++i) {
		std::ostringstream framePathStream;
		framePathStream << "vid/intro/frame_" << std::setw(4) << std::setfill('0') << i << ".png";
		std::string framePath = framePathStream.str();

		Texture* frameTexture = new Texture();
		frameTexture->loadFromFile(framePath, TEXTURE_PIXEL_FORMAT_RGBA);
		frameTextures.push_back(frameTexture);
	}
	PlaySound(TEXT("sound/intro.wav"), NULL, SND_FILENAME | SND_ASYNC);
	blackTexture = new Texture();
	blackTexture->loadFromFile("images/black.png", TEXTURE_PIXEL_FORMAT_RGBA);
	blackSprite = Sprite::createSprite(glm::ivec2(SCREEN_WIDTH, SCREEN_HEIGHT), glm::vec2(1, 1), blackTexture, &texProgram);
	blackSprite->setPosition(glm::vec2(0, 0));


}

void Scene::preLoadLevel(int nlives) {
	delete deadPlayer;
	deadPlayer = NULL;
	muerto = false;
	defaultSprite = NULL;
	defaultSprite2 = NULL;
	defaultSpritesheet = NULL;
	defaultSpritesheet2 = NULL;
	delete comida;
	comida = NULL;
	mapSelector = -1;
	hurrymusic = false;
	for (int i = 0; i < burbujas.size(); ++i) delete burbujas[i];
	for (int i = 0; i < platforms.size(); ++i) delete platforms[i];
	for (int i = 0; i < rope.size(); ++i) delete rope[i];
	for (int i = 0; i < powerup.size(); ++i) delete powerup[i];
	for (int i = 0; i < explosionRope.size(); ++i) delete explosionRope[i];
	for (int i = 0; i < spritesheetExplosionRope.size(); ++i) delete spritesheetExplosionRope[i];
	for (int i = 0; i < spriteexp.size(); ++i) delete spriteexp[i];
	for (int i = 0; i < spritesheetexp.size(); ++i) delete spritesheetexp[i];
	for (int i = 0; i < spritepts.size(); ++i) delete spritepts[i];
	for (int i = 0; i < spritesheetpts.size(); ++i) delete spritesheetpts[i];


	burbujas = std::vector<Bubble*>();
	spriteexp = std::vector<Sprite*>();
	spritesheetexp = std::vector<Texture*>();
	platforms = std::vector<Platform*>();
	contexp = std::vector<int>();
	rope = std::vector<Rope*>();
	petas = std::vector<bool>();
	rope2 = false;
	dinamita = false;
	contdin = 19;
	contcomida = 0;
	parat = false;
	powerup = std::vector<PowerUp*>();
	contpowerup = std::vector<int>();
	spritepts = std::vector<Sprite*>();
	spritesheetpts = std::vector<Texture*>();
	contpts = std::vector<int>();

	readySpritesheet = new Texture();
	readySpritesheet->loadFromFile("images/ready.png", TEXTURE_PIXEL_FORMAT_RGBA);
	readySprite = Sprite::createSprite(glm::ivec2(64, 20), glm::vec2(1, 0.5), readySpritesheet, &texProgram);
	readySprite->setNumberAnimations(1);
	readySprite->setAnimationSpeed(0, 8);
	readySprite->addKeyframe(0, glm::vec2(0, 0));
	readySprite->addKeyframe(0, glm::vec2(0, 0.5));
	readySprite->addKeyframe(0, glm::vec2(0, 0));
	readySprite->addKeyframe(0, glm::vec2(0, 0.5));
	readySprite->addKeyframe(0, glm::vec2(0, 0));
	readySprite->addKeyframe(0, glm::vec2(0, 0.5));
	readySprite->addKeyframe(0, glm::vec2(0, 0));
	readySprite->addKeyframe(0, glm::vec2(0, 0.5));
	readySprite->changeAnimation(0);
	readySprite->setPosition(glm::vec2(SCREEN_WIDTH*0.5f - 32, SCREEN_HEIGHT*0.5f - 10));
	passLevelTimer = 1000;
	deathTimer = 1000;
	lastpet = 0;
	lives = nlives;
	deathRagdollTimer = -777;
	deathTimer = 1000;
	muertoder = true;
	points = 0;
	
	mciSendString(L"stop stageCleared", NULL, 0, NULL);

	//interface
	livesTexture = new Texture();
	livesTexture->loadFromFile("images/lives.png", TEXTURE_PIXEL_FORMAT_RGBA);
	livesSprite = Sprite::createSprite(glm::ivec2(SCREEN_WIDTH/6, SCREEN_HEIGHT/2), glm::vec2(1, 1.f/3.f), livesTexture, &texProgram);
	livesSprite->setNumberAnimations(3);
	livesSprite->setAnimationSpeed(0, 1);
	livesSprite->addKeyframe(0, glm::vec2(0, 0));
	livesSprite->addKeyframe(1, glm::vec2(0, 1.f/3.f));
	livesSprite->addKeyframe(2, glm::vec2(0, 2.f/3.f));
	livesSprite->changeAnimation(lives-1);
	livesSprite->setPosition(glm::vec2(SCREEN_WIDTH*0.1, SCREEN_HEIGHT*0.25));
	//score
	scoreTexture = new Texture();
	scoreTexture->loadFromFile("images/score.png", TEXTURE_PIXEL_FORMAT_RGBA);
	scoreSprite = Sprite::createSprite(glm::ivec2(SCREEN_WIDTH / (12 * 4), SCREEN_HEIGHT / 4), glm::vec2(0.1f, 1.f), scoreTexture, &texProgram);
	scoreSprite->setNumberAnimations(10);
	scoreSprite->setAnimationSpeed(0, 1);
	scoreSprite->addKeyframe(0, glm::vec2(0, 0));
	scoreSprite->addKeyframe(1, glm::vec2(0.1f, 0));
	scoreSprite->addKeyframe(2, glm::vec2(0.2f, 0));
	scoreSprite->addKeyframe(3, glm::vec2(0.3f, 0));
	scoreSprite->addKeyframe(4, glm::vec2(0.4f, 0));
	scoreSprite->addKeyframe(5, glm::vec2(0.5f, 0));
	scoreSprite->addKeyframe(6, glm::vec2(0.6f, 0));
	scoreSprite->addKeyframe(7, glm::vec2(0.7f, 0));
	scoreSprite->addKeyframe(8, glm::vec2(0.8f, 0));
	scoreSprite->addKeyframe(9, glm::vec2(0.9f, 0));


	//timer
	timerTexture = new Texture();
	timerTexture->loadFromFile("images/timenumbers.png", TEXTURE_PIXEL_FORMAT_RGBA);
	timerSprite = Sprite::createSprite(glm::ivec2(8*2, 2*8), glm::vec2(0.1f, 1.f), timerTexture, &texProgram);
	timerSprite->setNumberAnimations(10);
	timerSprite->setAnimationSpeed(0, 1);
	timerSprite->addKeyframe(0, glm::vec2(0, 0));
	timerSprite->addKeyframe(1, glm::vec2(0.1f, 0));
	timerSprite->addKeyframe(2, glm::vec2(0.2f, 0));
	timerSprite->addKeyframe(3, glm::vec2(0.3f, 0));
	timerSprite->addKeyframe(4, glm::vec2(0.4f, 0));
	timerSprite->addKeyframe(5, glm::vec2(0.5f, 0));
	timerSprite->addKeyframe(6, glm::vec2(0.6f, 0));
	timerSprite->addKeyframe(7, glm::vec2(0.7f, 0));
	timerSprite->addKeyframe(8, glm::vec2(0.8f, 0));
	timerSprite->addKeyframe(9, glm::vec2(0.9f, 0));

	timerLettersTexture = new Texture();
	timerLettersTexture->loadFromFile("images/timeletters.png", TEXTURE_PIXEL_FORMAT_RGBA);
	timerLettersSprite = Sprite::createSprite(glm::ivec2(8*8,16), glm::vec2(1.f, 1.f), timerLettersTexture, &texProgram);
	timerLettersSprite->setNumberAnimations(1);
	timerLettersSprite->setAnimationSpeed(0, 1);
	timerLettersSprite->addKeyframe(0, glm::vec2(0, 0));
	timerLettersSprite->changeAnimation(0);
	timerLettersSprite->setPosition(glm::vec2(32*8+6, 10));





}

void Scene::credits() {
	cout << "Start credits" << endl;

	delete deadPlayer;
	deadPlayer = NULL;
	muerto = true;
	
	//clean
	
	defaultSprite = NULL;
	defaultSprite2 = NULL;
	defaultSpritesheet = NULL;
	defaultSpritesheet2 = NULL;
	delete comida;
	comida = NULL;
	mapSelector = -1;
	hurrymusic = false;
	delete player;
	player = NULL;

	for (int i = 0; i < burbujas.size(); ++i) delete burbujas[i];
	for (int i = 0; i < platforms.size(); ++i) delete platforms[i];
	for (int i = 0; i < rope.size(); ++i) delete rope[i];
	for (int i = 0; i < powerup.size(); ++i) delete powerup[i];
	for (int i = 0; i < explosionRope.size(); ++i) delete explosionRope[i];
	for (int i = 0; i < spritesheetExplosionRope.size(); ++i) delete spritesheetExplosionRope[i];
	for (int i = 0; i < spriteexp.size(); ++i) delete spriteexp[i];
	for (int i = 0; i < spritesheetexp.size(); ++i) delete spritesheetexp[i];
	for (int i = 0; i < spritepts.size(); ++i) delete spritepts[i];
	for (int i = 0; i < spritesheetpts.size(); ++i) delete spritesheetpts[i];
	burbujas = std::vector<Bubble*>();
	spriteexp = std::vector<Sprite*>();
	spritesheetexp = std::vector<Texture*>();
	platforms = std::vector<Platform*>();
	contexp = std::vector<int>();
	rope = std::vector<Rope*>();
	petas = std::vector<bool>();
	rope2 = false;
	dinamita = false;
	contdin = 19;
	contcomida = 0;
	parat = false;
	powerup = std::vector<PowerUp*>();
	contpowerup = std::vector<int>();
	spritepts = std::vector<Sprite*>();
	spritesheetpts = std::vector<Texture*>();
	contpts = std::vector<int>();

	//fondo
	credtutoTexture = new Texture();
	credtutoTexture->loadFromFile("images/credits.png", TEXTURE_PIXEL_FORMAT_RGBA);
	credtutoSprite = Sprite::createSprite(glm::ivec2(SCREEN_WIDTH, SCREEN_HEIGHT), glm::vec2(1, 1), credtutoTexture, &texProgram);
	credtutoSprite->setPosition(glm::vec2(0, 0));
	//music
	
	mciSendString(L"stop soundtrack1", NULL, 0, NULL);
	mciSendString(L"stop soundtrack2", NULL, 0, NULL);
	mciSendString(L"stop soundtrack3", NULL, 0, NULL);
	mciSendString(L"stop hurry", NULL, 0, NULL); mciSendString(L"stop credits", NULL, 0, NULL);
	

	mciSendString(L"open \"sound/credits.wav\" type mpegvideo alias credits", NULL, 0, NULL);
	mciSendString(L"seek credits to start", NULL, 0, NULL);
	mciSendString(L"play credits", NULL, 0, NULL);
	tutocredits = true;
	cout << "End credits" << endl;
}

void Scene::tutorial() {
	//clean
	delete deadPlayer;
	deadPlayer = NULL;
	muerto = true;
	defaultSprite = NULL;
	defaultSprite2 = NULL;
	defaultSpritesheet = NULL;
	defaultSpritesheet2 = NULL;
	delete comida;
	comida = NULL;
	mapSelector = -1;
	//delete player;
	//player = NULL;
	hurrymusic = false;
	for (int i = 0; i < burbujas.size(); ++i) delete burbujas[i];
	for (int i = 0; i < platforms.size(); ++i) delete platforms[i];
	for (int i = 0; i < rope.size(); ++i) delete rope[i];
	for (int i = 0; i < powerup.size(); ++i) delete powerup[i];
	for (int i = 0; i < explosionRope.size(); ++i) delete explosionRope[i];
	for (int i = 0; i < spritesheetExplosionRope.size(); ++i) delete spritesheetExplosionRope[i];
	for (int i = 0; i < spriteexp.size(); ++i) delete spriteexp[i];
	for (int i = 0; i < spritesheetexp.size(); ++i) delete spritesheetexp[i];
	for (int i = 0; i < spritepts.size(); ++i) delete spritepts[i];
	for (int i = 0; i < spritesheetpts.size(); ++i) delete spritesheetpts[i];
	burbujas = std::vector<Bubble*>();
	spriteexp = std::vector<Sprite*>();
	spritesheetexp = std::vector<Texture*>();
	platforms = std::vector<Platform*>();
	contexp = std::vector<int>();
	rope = std::vector<Rope*>();
	petas = std::vector<bool>();
	rope2 = false;
	dinamita = false;
	contdin = 19;
	contcomida = 0;
	parat = false;
	powerup = std::vector<PowerUp*>();
	contpowerup = std::vector<int>();
	spritepts = std::vector<Sprite*>();
	spritesheetpts = std::vector<Texture*>();
	contpts = std::vector<int>();
	//fondo
	credtutoTexture = new Texture();
	credtutoTexture->loadFromFile("images/tutorial.png", TEXTURE_PIXEL_FORMAT_RGBA);
	credtutoSprite = Sprite::createSprite(glm::ivec2(SCREEN_WIDTH, SCREEN_HEIGHT), glm::vec2(1, 1), credtutoTexture, &texProgram);
	credtutoSprite->setPosition(glm::vec2(0, 0));
	//music
	mciSendString(L"stop soundtrack1", NULL, 0, NULL);
	mciSendString(L"stop soundtrack2", NULL, 0, NULL);
	mciSendString(L"stop soundtrack3", NULL, 0, NULL);
	mciSendString(L"stop hurry", NULL, 0, NULL); mciSendString(L"stop credits", NULL, 0, NULL);

	mciSendString(L"open \"sound/credits.wav\" type mpegvideo alias credits", NULL, 0, NULL);
	mciSendString(L"seek credits to start", NULL, 0, NULL);
	mciSendString(L"play credits", NULL, 0, NULL);
	tutocredits = true;
	mapSelector = -1;
}

void Scene::loadMap() {
	delete stageClearedSprite;
	delete stageClearedTexture;
	stageClearedSprite = NULL;
	stageClearedTexture = NULL;
	delete credtutoSprite;
	delete credtutoTexture;
	credtutoSprite = NULL;
	credtutoTexture = NULL;
	delete map;
	map = NULL;
	comida = NULL;


	hurrymusic = false;
	tutocredits = false;

	defaultSpritesheet = new Texture();
	defaultSpritesheet2 = new Texture();
	defaultSpritesheet->loadFromFile("images/map.png", TEXTURE_PIXEL_FORMAT_RGBA);
	defaultSprite = Sprite::createSprite(glm::ivec2(SCREEN_WIDTH, SCREEN_HEIGHT), glm::vec2(1, 1), defaultSpritesheet, &texProgram);
	defaultSprite->setNumberAnimations(1);
	defaultSprite->setAnimationSpeed(0, 1);
	defaultSprite->addKeyframe(0, glm::vec2(0, 0));
	defaultSprite->changeAnimation(0);
	defaultSprite->setPosition(glm::vec2(0, 0));

	defaultSpritesheet2->loadFromFile("images/mapmarker.png", TEXTURE_PIXEL_FORMAT_RGBA);
	defaultSprite2 = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(1, 0.5), defaultSpritesheet2, &texProgram);
	defaultSprite2->setNumberAnimations(2);
	defaultSprite2->setAnimationSpeed(0, 2);
	defaultSprite2->addKeyframe(0, glm::vec2(0, 0));
	defaultSprite2->addKeyframe(0, glm::vec2(0, 0.5));
	defaultSprite2->changeAnimation(0);

	defaultSprite2->setAnimationSpeed(1, 10);
	defaultSprite2->addKeyframe(1, glm::vec2(0, 0));
	defaultSprite2->addKeyframe(1, glm::vec2(0, 0.5));
	defaultSprite2->addKeyframe(1, glm::vec2(0, 0));
	defaultSprite2->addKeyframe(1, glm::vec2(0, 0.5));
	defaultSprite2->addKeyframe(1, glm::vec2(0, 0));
	defaultSprite2->addKeyframe(1, glm::vec2(0, 0.5));
	defaultSprite2->addKeyframe(1, glm::vec2(0, 0));
	defaultSprite2->addKeyframe(1, glm::vec2(0, 0.5));
	defaultSprite2->addKeyframe(1, glm::vec2(0, 0));
	defaultSprite2->addKeyframe(1, glm::vec2(0, 0.5));
	vidFrame = -1;
	mapSelector = 0;
	muerto = true;

	mciSendString(L"stop soundtrack1", NULL, 0, NULL);
	mciSendString(L"stop soundtrack2", NULL, 0, NULL);
	mciSendString(L"stop soundtrack3", NULL, 0, NULL);
	mciSendString(L"stop hurry", NULL, 0, NULL); mciSendString(L"stop credits", NULL, 0, NULL);


	delete livesSprite;
	delete livesTexture;
	livesSprite = NULL;
	livesTexture = NULL;
	delete citySprite;
	delete cityTexture;
	citySprite = NULL;
	cityTexture = NULL;
	delete scoreSprite;
	delete scoreTexture;
	scoreSprite = NULL;
	scoreTexture = NULL;
	delete timerSprite;
	delete timerTexture;
	timerSprite = NULL;
	timerTexture = NULL;
	delete timerLettersSprite;
	delete timerLettersTexture;
	timerLettersSprite = NULL;
	timerLettersTexture = NULL;

	currentTime = 0.0f;





	
}

void Scene::passLevel() {
	/*
	delete timerTexture;
	delete timerSprite;
	timerTexture = NULL;
	timerSprite = NULL;
	*/
	mciSendString(L"stop soundtrack1", NULL, 0, NULL);
	mciSendString(L"stop soundtrack2", NULL, 0, NULL);
	mciSendString(L"stop soundtrack3", NULL, 0, NULL);
	mciSendString(L"stop hurry", NULL, 0, NULL); mciSendString(L"stop credits", NULL, 0, NULL);

	mciSendString(L"stop stageCleared", NULL, 0, NULL);
	mciSendString(L"open \"sound/stageCleared.wav\" type mpegvideo alias stageCleared", NULL, 0, NULL);
	mciSendString(L"seek stageCleared to start", NULL, 0, NULL);
	mciSendString(L"play stageCleared", NULL, 0, NULL);

	//fondo
	stageClearedTexture = new Texture();
	stageClearedTexture->loadFromFile("images/stageCleared.png", TEXTURE_PIXEL_FORMAT_RGBA);
	stageClearedSprite = Sprite::createSprite(glm::ivec2(SCREEN_WIDTH, SCREEN_HEIGHT), glm::vec2(1, 1), stageClearedTexture, &texProgram);
	stageClearedSprite->setPosition(glm::vec2(0, 0));

	//nums caca
	finalScoreTexture = new Texture();
	finalScoreTexture->loadFromFile("images/score.png", TEXTURE_PIXEL_FORMAT_RGBA);
	finalScoreSprite = Sprite::createSprite(glm::ivec2(SCREEN_WIDTH / (14 * 4), SCREEN_HEIGHT / 16), glm::vec2(0.1f, 1.f), finalScoreTexture, &texProgram);
	finalScoreSprite->setNumberAnimations(10);
	finalScoreSprite->setAnimationSpeed(0, 1);
	finalScoreSprite->addKeyframe(0, glm::vec2(0, 0));
	finalScoreSprite->addKeyframe(1, glm::vec2(0.1f, 0));
	finalScoreSprite->addKeyframe(2, glm::vec2(0.2f, 0));
	finalScoreSprite->addKeyframe(3, glm::vec2(0.3f, 0));
	finalScoreSprite->addKeyframe(4, glm::vec2(0.4f, 0));
	finalScoreSprite->addKeyframe(5, glm::vec2(0.5f, 0));
	finalScoreSprite->addKeyframe(6, glm::vec2(0.6f, 0));
	finalScoreSprite->addKeyframe(7, glm::vec2(0.7f, 0));
	finalScoreSprite->addKeyframe(8, glm::vec2(0.8f, 0));
	finalScoreSprite->addKeyframe(9, glm::vec2(0.9f, 0));



	stageClearedTimer = 3500;

}

void Scene::death() {
	deathWallSpritesheet = new Texture();
	deathWallSpritesheet->loadFromFile("images/deathwalls.png", TEXTURE_PIXEL_FORMAT_RGBA);
	deathWallSprite = Sprite::createSprite(glm::ivec2(SCREEN_WIDTH, SCREEN_HEIGHT), glm::vec2(1, 0.5f), deathWallSpritesheet, &texProgram);
	deathWallSprite->setPosition(glm::vec2(0, 0));
	deathWallSprite->setNumberAnimations(1);
	deathWallSprite->setAnimationSpeed(0, 10);
	deathWallSprite->addKeyframe(0, glm::vec2(0, 0));
	deathWallSprite->addKeyframe(0, glm::vec2(0, 0.5f));
	deathWallSprite->addKeyframe(0, glm::vec2(0, 0));
	deathWallSprite->addKeyframe(0, glm::vec2(0, 0.5f));
	deathWallSprite->changeAnimation(0);
	deathWallSprite->setPosition(glm::vec2(0, 0));
	deathTimer = -777;


	deadPlayer = new Bubble();
	deadPlayer->init(glm::ivec2(0,0), texProgram, 5, muertoder , false, true, true, player->getPosition().y-70, currentLevel);
	deadPlayer->setTileMap(map);

	deadPlayerSpritesheet = new Texture();
	deadPlayerSpritesheet->loadFromFile("images/deathPlayer.png", TEXTURE_PIXEL_FORMAT_RGBA);
	deadPlayer->sprite = Sprite::createSprite(glm::ivec2(48, 32), glm::vec2(1, 0.5), deadPlayerSpritesheet, &texProgram);
	deadPlayer->sprite->texture = deadPlayerSpritesheet;
	deadPlayer->sprite->setNumberAnimations(2);
	deadPlayer->sprite->setAnimationSpeed(0, 10);
	deadPlayer->sprite->addKeyframe(0, glm::vec2(0, 0));
	deadPlayer->sprite->addKeyframe(1, glm::vec2(0, 0.5f));
	if (player->lastLeft) deadPlayer->sprite->changeAnimation(1);
	else deadPlayer->sprite->changeAnimation(0);
	deadPlayer->setPosition(glm::ivec2(player->getPosition()));

	//sfx
	mciSendString(L"stop deathsfx", NULL, 0, NULL);
	mciSendString(L"open \"sound/death.wav\" type mpegvideo alias deathsfx", NULL, 0, NULL);
	mciSendString(L"seek deathsfx to start", NULL, 0, NULL);
	mciSendString(L"play deathsfx", NULL, 0, NULL);


	delete player;
	player = NULL;

	deathRagdollTimer = 2000;

	

}

void Scene::startLevel(int x, int lives) {
	if (x == 1) startLevel1(lives);
	else if (x == 2) startLevel2(lives);
	else if (x == 3) startLevel3(lives);
}

void Scene::startLevel1(int lives) {
	currentLevel = 1;
	preLoadLevel(lives);
	map = TileMap::createTileMap("levels/panglvl1.txt", glm::vec2(0, 0), texProgram, *this);
	player = new Player();
	player->init(glm::ivec2(0, 0), texProgram, *this);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	player->setTileMap(map);
	projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.f);
	timer = 100* 1000;

	//bubbles
	burbujas.push_back(new Bubble());
	burbujas[0]->init(glm::ivec2(0, 0), texProgram, 1, true, true, true, true, 3 * map->getTileSize(),currentLevel);
	burbujas[0]->setPosition(glm::vec2(5 * map->getTileSize(), 2 * map->getTileSize()));
	burbujas[0]->setTileMap(map);

	//fondo
	wallpaperSpritesheet = new Texture();
	wallpaperSpritesheet->loadFromFile("images/wallpaper1.png", TEXTURE_PIXEL_FORMAT_RGBA);
	wallpaperSprite = Sprite::createSprite(glm::ivec2(SCREEN_WIDTH, SCREEN_HEIGHT), glm::vec2(1, 1), wallpaperSpritesheet, &texProgram);
	wallpaperSprite->setPosition(glm::vec2(0, 0));

	//music
	mciSendString(L"stop soundtrack1", NULL, 0, NULL);
	mciSendString(L"stop soundtrack2", NULL, 0, NULL);
	mciSendString(L"stop soundtrack3", NULL, 0, NULL);
	mciSendString(L"stop hurry", NULL, 0, NULL); mciSendString(L"stop credits", NULL, 0, NULL);

	mciSendString(L"open \"sound/level1.wav\" type mpegvideo alias soundtrack1", NULL, 0, NULL);
	mciSendString(L"seek soundtrack1 to start", NULL, 0, NULL);
	mciSendString(L"play soundtrack1", NULL, 0, NULL);

	//city name
	cityTexture = new Texture();
	cityTexture->loadFromFile("images/fuji.png", TEXTURE_PIXEL_FORMAT_RGBA);
	citySprite = Sprite::createSprite(glm::ivec2(SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4), glm::vec2(1, 1), cityTexture, &texProgram);
	citySprite->setPosition(glm::vec2(SCREEN_WIDTH * (0.5 - 0.125), SCREEN_HEIGHT * 0.1));

	currentTime = 0.0f;

	
}

void Scene::startLevel3(int lives) {
	currentLevel = 3;
	preLoadLevel(lives);
	map = TileMap::createTileMap("levels/panglvl2.txt", glm::vec2(0, 0), texProgram, *this);
	player = new Player();
	player->init(glm::ivec2(0, 0), texProgram, *this);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	player->setTileMap(map);
	projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.f);
	timer = 90*1000;

	//bubbles
	burbujas.push_back(new Bubble());
	burbujas[0]->init(glm::ivec2(0, 0), texProgram, 1, true, true, true, true, 3 * map->getTileSize(), currentLevel);
	burbujas[0]->setPosition(glm::vec2(20 * map->getTileSize(), 2 * map->getTileSize()));
	burbujas[0]->setTileMap(map);

	burbujas.push_back(new Bubble());
	burbujas[1]->init(glm::ivec2(0, 0), texProgram, 1, false, true, true, true, 3 * map->getTileSize(), currentLevel);
	burbujas[1]->setPosition(glm::vec2(3 * map->getTileSize(), 2 * map->getTileSize()));
	burbujas[1]->setTileMap(map);


	//platforms
	for (int i = 0; i < 4; ++i) platforms.push_back(new Platform());
	platforms[0]->init(glm::ivec2(16 * 8, 8 * 8), texProgram, 1, map->map);
	platforms[1]->init(glm::ivec2(20 * 8, 8 * 8), texProgram, 1, map->map);
	platforms[2]->init(glm::ivec2(24 * 8, 8 * 8), texProgram, 1, map->map);
	platforms[3]->init(glm::ivec2(28 * 8, 8 * 8), texProgram, 1, map->map);
	for (int i = 0; i < 4; ++i) map->map[8 * 48 + 16 + i] = 9 + i;
	for (int i = 0; i < 4; ++i) map->map[8 * 48 + 20 + i] = 9 + i;
	for (int i = 0; i < 4; ++i) map->map[8 * 48 + 24 + i] = 9 + i;
	for (int i = 0; i < 4; ++i) map->map[8 * 48 + 28 + i] = 9 + i;

	//fondo
	wallpaperSpritesheet = new Texture();
	wallpaperSpritesheet->loadFromFile("images/wallpaper2.png", TEXTURE_PIXEL_FORMAT_RGBA);
	wallpaperSprite = Sprite::createSprite(glm::ivec2(SCREEN_WIDTH, SCREEN_HEIGHT), glm::vec2(1, 1), wallpaperSpritesheet, &texProgram);
	wallpaperSprite->setPosition(glm::vec2(0, 0));

	//music
	mciSendString(L"stop soundtrack1", NULL, 0, NULL);
	mciSendString(L"stop soundtrack2", NULL, 0, NULL);
	mciSendString(L"stop soundtrack3", NULL, 0, NULL);
	mciSendString(L"stop hurry", NULL, 0, NULL); mciSendString(L"stop credits", NULL, 0, NULL);
	mciSendString(L"open \"sound/level3.wav\" type mpegvideo alias soundtrack3", NULL, 0, NULL);
	mciSendString(L"seek soundtrack3 to start", NULL, 0, NULL);
	mciSendString(L"play soundtrack3", NULL, 0, NULL);


	//city name
	cityTexture = new Texture();
	cityTexture->loadFromFile("images/bcn.png", TEXTURE_PIXEL_FORMAT_RGBA);
	citySprite = Sprite::createSprite(glm::ivec2(SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4), glm::vec2(1, 1), cityTexture, &texProgram);
	citySprite->setPosition(glm::vec2(SCREEN_WIDTH * (0.5 - 0.125), SCREEN_HEIGHT * 0.1));

	currentLevel = 3;


}

void Scene::startLevel2(int lives) {
	currentLevel = 2;
	preLoadLevel(lives);
	map = TileMap::createTileMap("levels/panglvl3.txt", glm::vec2(0, 0), texProgram, *this);
	player = new Player();
	player->init(glm::ivec2(0, 0), texProgram, *this);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	player->setTileMap(map);
	projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.f);
	timer = 90*1000;
	

	burbujas.push_back(new Bubble());
	burbujas[0]->init(glm::ivec2(0, 0), texProgram, 1, true, true, true, true, 3 * map->getTileSize(), currentLevel);
	burbujas[0]->setPosition(glm::vec2(31 * map->getTileSize(), 2 * map->getTileSize()));
	burbujas[0]->setTileMap(map);

	burbujas.push_back(new Bubble());
	burbujas[1]->init(glm::ivec2(0, 0), texProgram, 3, true, true, true, true, 3 * map->getTileSize(), currentLevel);
	burbujas[1]->setPosition(glm::vec2(10 * map->getTileSize(), 13 * map->getTileSize()));
	burbujas[1]->setTileMap(map);

	burbujas.push_back(new Bubble());
	burbujas[2]->init(glm::ivec2(0, 0), texProgram, 3, true, true, true, true, 3 * map->getTileSize(), currentLevel);
	burbujas[2]->setPosition(glm::vec2(16* map->getTileSize(), 13 * map->getTileSize()));
	burbujas[2]->setTileMap(map);

	burbujas.push_back(new Bubble());
	burbujas[3]->init(glm::ivec2(0, 0), texProgram, 3, false, true, true, true, 3 * map->getTileSize(), currentLevel);
	burbujas[3]->setPosition(glm::vec2(39 * map->getTileSize(), 13 * map->getTileSize()));
	burbujas[3]->setTileMap(map);


	//platforms

	for (int i = 0; i < 2; ++i) platforms.push_back(new Platform());
	platforms[0]->init(glm::ivec2(11 * 8, 17 * 8), texProgram, 0, map->map);
	platforms[1]->init(glm::ivec2(28 * 8, 17 * 8), texProgram, 0, map->map);

	for (int i = 0; i < 3; ++i) map->map[17 * 48 + 11 + i] = 13 + i;
	for (int i = 0; i < 3; ++i) map->map[17 * 48 + 28 + i] = 13 + i;


	currentTime = 0.0f;
	for (int i = 0; i < burbujas.size(); ++i) burbujas[i]->stop = true;

	//fondo
	wallpaperSpritesheet = new Texture();
	wallpaperSpritesheet->loadFromFile("images/wallpaper3.png", TEXTURE_PIXEL_FORMAT_RGBA);
	wallpaperSprite = Sprite::createSprite(glm::ivec2(SCREEN_WIDTH, SCREEN_HEIGHT), glm::vec2(1, 1), wallpaperSpritesheet, &texProgram);
	wallpaperSprite->setPosition(glm::vec2(0, 0));

	//music
	mciSendString(L"stop soundtrack1", NULL, 0, NULL);
	mciSendString(L"stop soundtrack2", NULL, 0, NULL);
	mciSendString(L"stop soundtrack3", NULL, 0, NULL);
	mciSendString(L"stop hurry", NULL, 0, NULL); mciSendString(L"stop credits", NULL, 0, NULL);
	mciSendString(L"open \"sound/level2.wav\" type mpegvideo alias soundtrack2", NULL, 0, NULL);
	mciSendString(L"seek soundtrack2 to start", NULL, 0, NULL);
	mciSendString(L"play soundtrack2", NULL, 0, NULL);


	//city name
	cityTexture = new Texture();
	cityTexture->loadFromFile("images/london.png", TEXTURE_PIXEL_FORMAT_RGBA);
	citySprite = Sprite::createSprite(glm::ivec2(SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4), glm::vec2(1, 1), cityTexture, &texProgram);
	citySprite->setPosition(glm::vec2(SCREEN_WIDTH * (0.5 - 0.125), SCREEN_HEIGHT * 0.1));

	currentLevel = 2;

}

void Scene::update(int deltaTime)
{

	if (tutocredits && Game::instance().getKey(GLFW_KEY_SPACE)) {
		delete credtutoSprite;
		delete credtutoTexture;
		credtutoSprite = NULL;
		credtutoTexture = NULL;
		loadMap();
		return;
	}
	if (timer > 0 && timer != -777) timer -= deltaTime;
	else muerto = true;
	if (timer < 50000 && !hurrymusic && !muerto && player!= NULL) {
		hurrymusic = true;
		mciSendString(L"stop soundtrack1", NULL, 0, NULL);
		mciSendString(L"stop soundtrack2", NULL, 0, NULL);
		mciSendString(L"stop soundtrack3", NULL, 0, NULL);
		mciSendString(L"stop hurry", NULL, 0, NULL); mciSendString(L"stop credits", NULL, 0, NULL);

		mciSendString(L"open \"sound/hurry.wav\" type mpegvideo alias hurry", NULL, 0, NULL);
		mciSendString(L"seek hurry to start", NULL, 0, NULL);
		mciSendString(L"play hurry", NULL, 0, NULL);
	}

	if (deathRagdollTimer != -777) {
		if (deathRagdollTimer > 0) deathRagdollTimer -= deltaTime;
		else {
			deathRagdollTimer = -777;
			delete deadPlayer;
			deadPlayer = NULL;
			if (--lives > 0) startLevel(currentLevel, lives);
			else loadMap();
			
		}
	}
	if (godMode) { //godmode blink
		godModeBlinkTimer -= deltaTime;
		if (godModeBlinkTimer <= 0) {
			godModeBlinkTimer = 75;
			visible = !visible;
		}
	}

	if (deadPlayer != NULL) {
		deadPlayer->update(deltaTime);
		//if (deadPlayer->)
	}
	if (readySprite != NULL) {
		readySprite->update(deltaTime);
		if (readySprite->currentKeyframe == 7) {
			delete readySprite;
			delete readySpritesheet;
			readySprite = NULL;
		}
		else return;
	}
	if (burbujas.size() == 0 && player != NULL && mapSelector == -1) {
		passLevelTimer -= deltaTime;
		if (passLevelTimer <= 0 && stageClearedTimer == -777) {
			stageClearedTimer = -777;
			passLevel();
			return;
		}
	}
	if (deathWallSprite != NULL) {
		deathWallSprite->update(deltaTime);
		if (deathWallSprite->currentKeyframe == 3) {
			delete deathWallSprite;
			delete deathWallSpritesheet;
			deathWallSprite = NULL;
		}
	}
	else if (burbujas.size() != 0 && (muerto ) && passLevelTimer == 1000 && deathTimer != -777) { // death
		if (deathTimer >= 0) deathTimer -= deltaTime;
		else death();
	}
	if (!muerto) {
		currentTime += deltaTime;
		if (player != NULL) player->update(deltaTime);
		for (int i = 0; i < rope.size(); ++i) {
			if (!rope[i]->update(deltaTime) || petas[i]) {
				delete rope[i];

				for (int w = i; w < rope.size() - 1; ++w) {
					rope[w] = rope[w + 1];
					petas[w] = petas[w + 1];
				}
				rope.pop_back();
				petas.pop_back();
			}
		}
		this->updateBurbujas(deltaTime);
		for (int i = 0; i < powerup.size(); ++i) {
			powerup[i]->update(deltaTime);
			contpowerup[i]++;
		}

		if (comida != NULL) {
			comida->update(deltaTime);
			++contcomida;
		}

		if (comida != NULL && contcomida == 350) {
			delete comida;
			comida = NULL;
			contcomida = 0;
		}
		for (int j = 0; j < contpowerup.size(); ++j) {
			if (contpowerup[j] == 350) {

				delete powerup[j];
				for (int w = j; w < powerup.size() - 1; ++w) {
					powerup[w] = powerup[w + 1];
					contpowerup[w] = contpowerup[w + 1];
				}

				contpowerup.pop_back();
				powerup.pop_back();
			}
		}

		glm::ivec2 pp = player->getPosition();


		if (comida != NULL) {
			glm::vec2 ppu = comida->getpos();
			int id = comida->in;
			if (circleRect(pp.x + 32 / 2, pp.y + 32 / 2, 20 / 2, ppu.x, ppu.y, 32, 32)) {


				mciSendString(L"stop itemsfx", NULL, 0, NULL);
				mciSendString(L"open \"sound/item.wav\" type mpegvideo alias itemsfx", NULL, 0, NULL);
				mciSendString(L"seek itemsfx to start", NULL, 0, NULL);
				mciSendString(L"play itemsfx", NULL, 0, NULL);
				
				int paux = comida->pointscomida[id];
				points += paux;

				std::vector<int> unidades;

				while (paux > 0) {
					int unidad = paux % 10; // Obtiene la �ltima cifra
					unidades.insert(unidades.begin(), unidad); // Inserta la unidad al principio del vector
					paux /= 10; // Elimina la �ltima cifra del n�mero
				}
				for (int i = unidades.size() - 1; i >= 0; --i) {
					spritesheetpts.push_back(new Texture());
					int j = spritesheetpts.size() - 1;
					spritesheetpts[j]->loadFromFile("images/" + std::to_string(unidades[i]) + ".png", TEXTURE_PIXEL_FORMAT_RGBA);

					spritepts.push_back(Sprite::createSprite(glm::ivec2(4, 6), glm::vec2(1.f, 1.f), spritesheetpts[j], &texProgram));
					spritepts[j]->setNumberAnimations(1);
					spritepts[j]->setPosition(glm::vec2(float((ppu.x+10) + 4 * i), float(ppu.y + 24)));

					contpts.push_back(0);
				}

				delete comida;
				comida = NULL;
				contcomida = 0;
				


			}

		}

		for (int j = 0; j < powerup.size(); ++j) {
			glm::vec2 ppu = powerup[j]->getpos();
			int cop = powerup[j]->cpu;
			int id = powerup[j]->in;
			int x = 16;
			
			if (circleRect(pp.x + 32 / 2, pp.y + 32 / 2, 20 / 2, ppu.x, ppu.y, x, x)) {


				mciSendString(L"stop itemsfx", NULL, 0, NULL);
				mciSendString(L"open \"sound/item.wav\" type mpegvideo alias itemsfx", NULL, 0, NULL);
				mciSendString(L"seek itemsfx to start", NULL, 0, NULL);
				mciSendString(L"play itemsfx", NULL, 0, NULL);
				int cop = powerup[j]->cpu;
				int id = powerup[j]-> in;
				if (cop == 0) {

					points += 100;

					if (id == 1) {
						dinamita = true;
					}
					else if (id == 2) parat = true;

					else { rope2 = true; }
				}



				delete powerup[j];
				for (int w = j; w < powerup.size() - 1; ++w) {
					powerup[w] = powerup[w + 1];
					contpowerup[w] = contpowerup[w + 1];
				}

				contpowerup.pop_back();
				powerup.pop_back();


			}

		}
	}

	for (int i = 0; i < platforms.size(); ++i) {
		platforms[i]->update(deltaTime);
		if (platforms[i]->sprite->currentKeyframe == 4 && platforms[i]->sprite->currentAnimation == 1) {
			delete platforms[i];
			for (int w = i; w < platforms.size() - 1; ++w) { platforms[w] = platforms[w + 1]; }
			platforms.pop_back();
			--i;
		}
	}

	for (int i = 0; i < explosionRope.size(); ++i) {
		explosionRope[i]->update(deltaTime);
		if (explosionRope[i]->currentKeyframe == 3) {
			delete explosionRope[i];
			delete spritesheetExplosionRope[i];
			for (int w = i; w < explosionRope.size() - 1; ++w) { explosionRope[w] = explosionRope[w + 1]; }
			explosionRope.pop_back();

			for (int w = i; w < spritesheetExplosionRope.size() - 1; ++w) { spritesheetExplosionRope[w] = spritesheetExplosionRope[w + 1]; }
			spritesheetExplosionRope.pop_back();

		}

	}

	for (int i = 0; i < spritepts.size(); ++i) {
		spritepts[i]->update(deltaTime);
		contpts[i] += 1;

		if (contpts[i] == 100) {
			delete spritepts[i];
			delete spritesheetpts[i];
			for (int w = i; w < spritepts.size() - 1; ++w) { spritepts[w] = spritepts[w + 1]; }
			spritepts.pop_back();
			for (int w = i; w < spritesheetpts.size() - 1; ++w) { spritesheetpts[w] = spritesheetpts[w + 1]; }
			spritesheetpts.pop_back();
			for (int w = i; w < contpts.size() - 1; ++w) { contpts[w] = contpts[w + 1]; }
			contpts.pop_back();

		}

	}

	//intro video

	if (defaultSprite != NULL) {
		if (vidFrame != -1) { //readyintro animation
			if (vidFrame < 118) defaultSprite->texture = (frameTextures[vidFrame++ / 2]);
			else {

				vidFrame = -1;
				defaultSpritesheet->loadFromFile("images/intro.png", TEXTURE_PIXEL_FORMAT_RGBA);
				defaultSprite = Sprite::createSprite(glm::ivec2(SCREEN_WIDTH, SCREEN_HEIGHT), glm::vec2(1, 0.5f), defaultSpritesheet, &texProgram);
				defaultSprite->setNumberAnimations(2);
				defaultSprite->setAnimationSpeed(0, 1);
				defaultSprite->addKeyframe(0, glm::vec2(0, 0));
				defaultSprite->addKeyframe(0, glm::vec2(0, 0.5f));
				defaultSprite->changeAnimation(0);
				defaultSprite->setPosition(glm::vec2(0, 0));

				//anim click
				defaultSprite->setAnimationSpeed(1, 10);
				defaultSprite->addKeyframe(1, glm::vec2(0, 0));
				defaultSprite->addKeyframe(1, glm::vec2(0, 0.5f));
				defaultSprite->addKeyframe(1, glm::vec2(0, 0));
				defaultSprite->addKeyframe(1, glm::vec2(0, 0.5f));
				defaultSprite->addKeyframe(1, glm::vec2(0, 0));
				defaultSprite->addKeyframe(1, glm::vec2(0, 0.5f));
				defaultSprite->addKeyframe(1, glm::vec2(0, 0));
				defaultSprite->addKeyframe(1, glm::vec2(0, 0.5f));
				defaultSprite->addKeyframe(1, glm::vec2(0, 0));
				defaultSprite->addKeyframe(1, glm::vec2(0, 0.5f));
				defaultSprite->addKeyframe(1, glm::vec2(0, 0));
				defaultSprite->addKeyframe(1, glm::vec2(0, 0.5f));
			}
		}
		else if (mapSelector == -1) { //press space to play
			defaultSprite->update(deltaTime);
			if (Game::instance().getKey(GLFW_KEY_SPACE)) {
				if (defaultSprite->currentAnimation == 0) defaultSprite->changeAnimation(1);
			}
			if (defaultSprite->currentKeyframe == 11) { //load map
				loadMap();
			}
		}
		else if (mapSelector != -1) { //map selector
			defaultSprite2->update(deltaTime);
			if (clickCd > 0) clickCd -= deltaTime;
			if (mapSelector == 0) defaultSprite2->setPosition(glm::vec2(SCREEN_WIDTH * 0.818f, SCREEN_HEIGHT * 0.323f));
			else if (mapSelector == 1) defaultSprite2->setPosition(glm::vec2(SCREEN_WIDTH * 0.4067f, SCREEN_HEIGHT * 0.215f));
			else if (mapSelector == 2) defaultSprite2->setPosition(glm::vec2(SCREEN_WIDTH * 0.3615f, SCREEN_HEIGHT * 0.31f)); //37, 30
			else if (mapSelector == 3) defaultSprite2->setPosition(glm::vec2(SCREEN_WIDTH * 0.3615f, SCREEN_HEIGHT * 0.6f));
			else if (mapSelector == 4) defaultSprite2->setPosition(glm::vec2(SCREEN_WIDTH * 0.65f, SCREEN_HEIGHT * 0.60f));

			if (Game::instance().getKey(GLFW_KEY_LEFT) && mapSelector < 4 && clickCd <= 0) {
				++mapSelector;
				clickCd = 135;
				defaultSprite2->changeAnimation(0);
			}
			else if (Game::instance().getKey(GLFW_KEY_RIGHT) && mapSelector > 0 && clickCd <= 0) {
				--mapSelector;
				clickCd = 135;
				defaultSprite2->changeAnimation(0);
			}
			if (Game::instance().getKey(GLFW_KEY_SPACE) && clickCd <= 0) defaultSprite2->changeAnimation(1); //space clicked
			if (defaultSprite2->currentAnimation == 1 && defaultSprite2->currentKeyframe == 9) { //charge level
				cout << mapSelector << endl;
				if (mapSelector == 0) startLevel1(3);
				else if (mapSelector == 1) startLevel2(3);
				else if (mapSelector == 2) startLevel3(3);
				else if (mapSelector == 3) tutorial();
				else if (mapSelector == 4) credits();
			}
		}

		

	}
	
}
	



void Scene::render()
{
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	int screenWidth, screenHeight;
	glfwGetWindowSize(Game::instance().window, &screenWidth, &screenHeight);

	if ((float)screenWidth / (float)screenHeight > (float)SCREEN_WIDTH / (float)(SCREEN_HEIGHT + 34)) { //+ ample
		int newWidth = SCREEN_WIDTH * screenHeight / (SCREEN_HEIGHT+34);
		glViewport((screenWidth - newWidth) / 2, screenHeight*(1.f-(208.0f / 242.f)), newWidth, screenHeight * (208.0f / 242.f));
	}
	else {
		int newHeight = (SCREEN_HEIGHT+34)*screenWidth / SCREEN_WIDTH;
		glViewport(0, (screenHeight - newHeight) / 2 + newHeight * (1.f - (208.0f / 242.f)), screenWidth, newHeight * (208.0f / 242.f));
	}
	



	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);


	
	if (stageClearedSprite != NULL) {
		stageClearedSprite->render();
		
		if (finalTimer == -777) {
			finalTimer = (timer / 1000)*100;
			finalScore = finalTimer + points;
		}
		
		for (int i = 0; i < 5; ++i) {
			int scoreaux = finalTimer;
			for (int j = i; j < 4; ++j) scoreaux /= 10;
			scoreaux %= 10;
			finalScoreSprite->changeAnimation(scoreaux);
			finalScoreSprite->setPosition(glm::vec2(26 * 8 + i * 8 + 6, 18 * 8));
			finalScoreSprite->render();
		}
		
		for (int i = 0; i < 5; ++i) {
			int scoreaux = finalScore;
			for (int j = i; j < 4; ++j) scoreaux /= 10;
			scoreaux %= 10;
			finalScoreSprite->changeAnimation(scoreaux);
			finalScoreSprite->setPosition(glm::vec2(22 * 8 + i * 8 + 6, 21*8));
			finalScoreSprite->render();
		}


		if (stageClearedTimer <= 0) {
			delete stageClearedSprite;
			delete stageClearedTexture;
			stageClearedSprite = NULL;
			stageClearedTexture = NULL;
			stageClearedTimer = -777;
			finalTimer = -777;
			finalScore = -777;
			loadMap();
		}
		return;
	}
	
	if (wallpaperSprite != NULL) wallpaperSprite->render();
	if (map != NULL) map->render();
	if (deathWallSprite != NULL) deathWallSprite->render();
	for (int i = 0; i < rope.size();++i) rope[i]->render();
	for (int i = 0; i < explosionRope.size() ; ++i) {
		explosionRope[i]->render();

	}
	if (player != NULL && visible) player->render();
	for (int i = 0; i < burbujas.size(); ++i) {
		if (parat && contparat % 410 > 310) {
			if ((410 - (contparat % 410))/4 % 2 == 0) burbujas[i]->render();
		}
		else burbujas[i]->render();
	}
	for (int i = 0; i < spriteexp.size(); ++i) {
		spriteexp[i]->render();
	}
	for (int i = 0; i < platforms.size(); ++i) {
		platforms[i]->render();
	}

	for (int i = 0; i < spritepts.size(); ++i) {
		spritepts[i]->render();
	}

	for (int i = 0; i < powerup.size(); ++i) {
		if (350-contpowerup[i] <= 60) {
			if ((350 - contpowerup[i]) / 4 % 2 == 0) powerup[i]->render();
		}
		else powerup[i]->render();
		
	}

	if (comida != NULL) {

		if (350 - contcomida <= 60) {
			if ((350 - contcomida) / 4 % 2 == 0) comida->render();
		}
		else comida->render();
	}

	if (defaultSprite != NULL) defaultSprite->render();
	if (defaultSprite2 != NULL) defaultSprite2->render();
	if (readySprite != NULL) readySprite->render();
	if (deadPlayer != NULL) deadPlayer->render();





	//timer
	if (timerLettersSprite != NULL) timerLettersSprite->render();
	if (timerSprite != NULL) {
		for (int i = 0; i < 3; ++i) {
			int timeaux = timer/1000;
			for (int j = i; j < 2; ++j) timeaux /= 10;
			timeaux %= 10;
			timerSprite->changeAnimation(timeaux);
			timerSprite->setPosition(glm::vec2(40 * 8 + i * 16+6, 10));
			timerSprite->render();
		
		}
	}


	if (credtutoSprite != NULL) {
		credtutoSprite->render();
		//return;
	}

	

	glfwGetWindowSize(Game::instance().window, &screenWidth, &screenHeight);

	if ((float)screenWidth / (float)screenHeight > (float)SCREEN_WIDTH / (float)(SCREEN_HEIGHT + 34)) { //+ ample
		int newWidth = SCREEN_WIDTH * screenHeight / (SCREEN_HEIGHT + 34);
		glViewport((screenWidth - newWidth) / 2, 0, newWidth, screenHeight * (34.f / 242.f));
	}
	else {
		int newHeight = (SCREEN_HEIGHT + 34) * screenWidth / SCREEN_WIDTH;
		glViewport(0, (screenHeight - newHeight) / 2, screenWidth, newHeight * (34.f / 242.f));
	}

	

	if (blackSprite != NULL) blackSprite->render();
	if (livesSprite != NULL) livesSprite->render();
	if (citySprite != NULL) citySprite->render();
	if (scoreSprite != NULL) {
		for (int i = 0; i < 5; ++i) {
			int scoreaux = points;
			for (int j = i; j < 5; ++j) scoreaux /= 10;
			scoreaux %= 10;			
			scoreSprite->changeAnimation(scoreaux);
			scoreSprite->render();
			scoreSprite->setPosition(glm::vec2(SCREEN_WIDTH * 0.8 + (SCREEN_WIDTH / (12 * 4)) * i, SCREEN_HEIGHT * 0.4));
		}
	}
	
}

void Scene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if(!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if(!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if(!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}

bool Scene::circleRect(int cx, int cy, float radius, float rx, float ry, float rw, float rh) const {

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



bool Scene::createRope(glm::ivec2 pos)
{
	if ((!rope2 && rope.size()==1) || (rope2 &&rope.size() == 2)) return false;
	rope.push_back(new Rope());
	int j = rope.size() - 1;
	rope[j]->init(glm::ivec2(0, 0), texProgram, glm::ivec2(pos.x, pos.y), *map);

	spritesheetExplosionRope.push_back(new Texture());
	j = spritesheetExplosionRope.size() - 1;
	spritesheetExplosionRope[j]->loadFromFile("images/explosionrope.png", TEXTURE_PIXEL_FORMAT_RGBA);

	explosionRope.push_back(Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(1.f / 4.f, 1.f), spritesheetExplosionRope[j], &texProgram));
	explosionRope[j]->setNumberAnimations(1);
	explosionRope[j]->setAnimationSpeed(0, 20);
	for (int i = 0; i < 4; i++) explosionRope[j]->addKeyframe(0, glm::vec2(i / 4.f, 0));
	explosionRope[j]->changeAnimation(0);
	explosionRope[j]->setPosition(glm::vec2(float(pos.x - 8), float(pos.y - 29 - 16)));

	petas.push_back(false);

	mciSendString(L"stop dispar", NULL, 0, NULL);
	mciSendString(L"open \"sound/dispar.wav\" type mpegvideo alias dispar", NULL, 0, NULL);
	mciSendString(L"seek dispar to start", NULL, 0, NULL);
	mciSendString(L"play dispar", NULL, 0, NULL);

	return true;
}


void Scene::updateBurbujas(int deltaTime)
{
	if (currentLevel == -1) return;
	if (godCounter > 0) godCounter -= deltaTime;
	++cont;
	if (dinamita) ++contdin;
	if (stageClearedTimer != -777) stageClearedTimer -= deltaTime;

	if (parat) ++contparat;
	
	currentTime += deltaTime;

	std::random_device rd;
	std::mt19937 gen(rd()); // Mersenne Twister

	std::uniform_int_distribution<> dist(1, 800);
	int randomNumber = dist(gen);

	if (randomNumber == 1) {
		std::uniform_int_distribution<> dist3(1, 210);
		int randomNumber = dist3(gen);
		bool pepeaux = false;
		
		if (comida == NULL) {

			comida = new PowerUp();
			contcomida = 0;
			int auxi = 0;
			if (randomNumber <= 60) auxi = 1;
			else if (randomNumber <= 110) auxi = 2;
			else if (randomNumber <= 150) auxi = 3;
			else if (randomNumber <= 180) auxi = 4;
			else if (randomNumber <= 200) auxi = 5;
			else if (randomNumber <= 210) auxi = 6;

			comida->init(glm::ivec2(0, 0), texProgram, 1, auxi);
			
			std::uniform_int_distribution<> dist2(1, 3);
			randomNumber = dist2(gen);
			if (randomNumber <= 1) auxi = 60;
			else if (randomNumber <= 2) auxi = 180;
			else if (randomNumber <= 3) auxi = 294;


			comida->setPosition(glm::vec2(auxi, 2));
			comida->setTileMap(map);
		
		}
	}

	for (int i = 0; i < spriteexp.size(); ++i) {

		++contexp[i];

		if (contexp[i] > 16) {
			delete spriteexp[i];
			delete spritesheetexp[i];
			for (int w = i; w < spriteexp.size() - 1; ++w) { spriteexp[w] = spriteexp[w + 1]; }
			spriteexp.pop_back();
			for (int w = i; w < spritesheetexp.size() - 1; ++w) { spritesheetexp[w] = spritesheetexp[w + 1]; }
			spritesheetexp.pop_back();
			for (int w = i; w < contexp.size() - 1; ++w) { contexp[w] = contexp[w + 1]; }
			contexp.pop_back();

		}
		else spriteexp[i]->update(deltaTime);
	}
	bool pet1 = false;
	for (int i = 0; i < burbujas.size(); ++i) {

		if (parat) burbujas[i]->stop = true;
		if (contparat % 410 == 0) {
			if(i == burbujas.size()-1) parat = false;
			burbujas[i]->stop = false;
		}
		burbujas[i]->update(deltaTime);
		
		int mode = burbujas[i]->getmode();
		glm::vec2 posp = burbujas[i]->getpos();
		int wact = burbujas[i]->getw(mode);
		int hact = burbujas[i]->geth(mode);

		glm::ivec2 pp = player->getPosition();

		if (circleRect(posp.x + wact / 2, posp.y + hact / 2, wact / 2, pp.x + 12, pp.y + 8, 8, 18) && !godMode) {
			muerto = true;
			if (posp.x + wact / 2 > pp.x + 16) {
				

				muertoder = false;
			}
			else {
				muertoder = true;
			}
			
		}
		int xr, yir,yar;

		

		
		
		for (int z = 0; z < rope.size() || (dinamita && contdin == 20 && mode != 4) ;++z) {

			if (rope.size() != 0) {
				xr = rope[z]->getx();
				yir = rope[z]->getyini();
				yar = rope[z]->getyact();
			}

			if ((rope.size() != 0 && circleRect(posp.x + wact / 2, posp.y + hact / 2, wact / 2, xr - 4, yar, 9, yir - yar) && !pet1) || (dinamita && contdin == 20 && mode != 4)) {


				points += ptsbb[mode];


				if ((rope.size() != 0) && circleRect(posp.x + wact / 2, posp.y + hact / 2, wact / 2, xr - 4, yar, 9, yir - yar) && !pet1) {
					petas[z] = true;

					if (mode == lastpet) {
						points -= ptsbb[mode];
						int paux = ptsbb[mode] * multbb;
						points += paux;

						std::vector<int> unidades;

						while (paux > 0) {
							int unidad = paux % 10; // Obtiene la �ltima cifra
							unidades.insert(unidades.begin(), unidad); // Inserta la unidad al principio del vector
							paux /= 10; // Elimina la �ltima cifra del n�mero
						}
						for (int i = unidades.size() - 1; i >= 0; --i) {
							spritesheetpts.push_back(new Texture());
							int j = spritesheetpts.size() - 1;
							spritesheetpts[j]->loadFromFile("images/" + std::to_string(unidades[i]) + ".png", TEXTURE_PIXEL_FORMAT_RGBA);

							spritepts.push_back(Sprite::createSprite(glm::ivec2(4, 6), glm::vec2(1.f, 1.f), spritesheetpts[j], &texProgram));
							spritepts[j]->setNumberAnimations(1);
							spritepts[j]->setPosition(glm::vec2(float((posp.x + wact / 2 -10) + 4 * i), float(posp.y + hact / 2)));

							contpts.push_back(0);
						}
						if (multbb < 8) multbb *= 2;

					}

					else {
						multbb = 2;
						lastpet = mode;
					}
				}
				pet1 = true;
				mciSendString(L"stop pang", NULL, 0, NULL);
				mciSendString(L"open \"sound/pang.wav\" type mpegvideo alias pang", NULL, 0, NULL);
				mciSendString(L"seek pang to start", NULL, 0, NULL);
				mciSendString(L"play pang", NULL, 0, NULL);
				int wsig = burbujas[i]->getw(mode + 1);
				int hsig = burbujas[i]->geth(mode + 1);
				glm::vec2 post = burbujas[i]->getpostot();
				delete burbujas[i];



				int min = 1;
				int max = 3;
				if (dinamita) {
					min = 1;
					max = 10;
				}
				std::uniform_int_distribution<> dist(min, max);
				int randomNumber = dist(gen);

				if (randomNumber == 1 && mode != 4) {
					std::uniform_int_distribution<> dist(min, 3);
					int randomNumber = dist(gen);
					powerup.push_back(new PowerUp());
					contpowerup.push_back(0);
					int j = powerup.size() - 1;
					powerup[j]->init(glm::ivec2(0, 0), texProgram, 0, randomNumber);
					powerup[j]->setPosition(glm::vec2(posp.x + wact / 2 - 8, posp.y + hact / 2 - 8));
					powerup[j]->setTileMap(map);
				}


				if (mode == 1) {
					spritesheetexp.push_back(new Texture());
					int j = spritesheetexp.size() - 1;
					if (currentLevel == 2) spritesheetexp[j]->loadFromFile("images/explosion.png", TEXTURE_PIXEL_FORMAT_RGBA);
					else spritesheetexp[j]->loadFromFile("images/explosion2.png", TEXTURE_PIXEL_FORMAT_RGBA);
					spriteexp.push_back(Sprite::createSprite(glm::ivec2(48, 46), glm::vec2(0.25, 0.25), spritesheetexp[j], &texProgram));

					spriteexp[j]->setNumberAnimations(1);
					spriteexp[j]->setAnimationSpeed(0, 15);
					spriteexp[j]->addKeyframe(0, glm::vec2(0.f, 0.0f));
					spriteexp[j]->addKeyframe(0, glm::vec2(0.25f, 0.0));
					spriteexp[j]->addKeyframe(0, glm::vec2(0.5f, 0.0f));
					spriteexp[j]->addKeyframe(0, glm::vec2(0.75f, 0.0f));
					spriteexp[j]->changeAnimation(0);
					spriteexp[j]->setPosition(post);
					contexp.push_back(0);
				}
				else if (mode == 2) {
					spritesheetexp.push_back(new Texture());
					int j = spritesheetexp.size() - 1;
					if (currentLevel == 2) spritesheetexp[j]->loadFromFile("images/explosion.png", TEXTURE_PIXEL_FORMAT_RGBA);
					else spritesheetexp[j]->loadFromFile("images/explosion2.png", TEXTURE_PIXEL_FORMAT_RGBA);
					spriteexp.push_back(Sprite::createSprite(glm::ivec2(48, 46), glm::vec2(0.25, 0.25), spritesheetexp[j], &texProgram));

					spriteexp[j]->setNumberAnimations(1);
					spriteexp[j]->setAnimationSpeed(0, 15);
					spriteexp[j]->addKeyframe(0, glm::vec2(0.f, 0.25f));
					spriteexp[j]->addKeyframe(0, glm::vec2(0.25f, 0.25f));
					spriteexp[j]->addKeyframe(0, glm::vec2(0.5f, 0.25f));
					spriteexp[j]->addKeyframe(0, glm::vec2(0.75f, 0.25f));
					spriteexp[j]->changeAnimation(0);
					spriteexp[j]->setPosition(glm::vec2(post.x,post.y));
					contexp.push_back(0);
				}
				else if (mode == 3) {
					spritesheetexp.push_back(new Texture());
					int j = spritesheetexp.size() - 1;
					if (currentLevel == 2) spritesheetexp[j]->loadFromFile("images/explosion.png", TEXTURE_PIXEL_FORMAT_RGBA);
					else spritesheetexp[j]->loadFromFile("images/explosion2.png", TEXTURE_PIXEL_FORMAT_RGBA);
					spriteexp.push_back(Sprite::createSprite(glm::ivec2(48, 46), glm::vec2(0.25, 0.25), spritesheetexp[j], &texProgram));

					spriteexp[j]->setNumberAnimations(1);
					spriteexp[j]->setAnimationSpeed(0, 15);
					spriteexp[j]->addKeyframe(0, glm::vec2(0.f, 0.5f));
					spriteexp[j]->addKeyframe(0, glm::vec2(0.25f, 0.5f));
					spriteexp[j]->addKeyframe(0, glm::vec2(0.5f, 0.5f));
					spriteexp[j]->addKeyframe(0, glm::vec2(0.75f, 0.5f));
					spriteexp[j]->changeAnimation(0);
					spriteexp[j]->setPosition(glm::vec2(post.x + 15, post.y + 15));
					contexp.push_back(0);
				}
				else if (mode == 4) {
					spritesheetexp.push_back(new Texture());
					int j = spritesheetexp.size() - 1;
					if(currentLevel == 2) spritesheetexp[j]->loadFromFile("images/explosion.png", TEXTURE_PIXEL_FORMAT_RGBA);
					else spritesheetexp[j]->loadFromFile("images/explosion2.png", TEXTURE_PIXEL_FORMAT_RGBA);
					spriteexp.push_back(Sprite::createSprite(glm::ivec2(48, 46), glm::vec2(0.25, 0.25), spritesheetexp[j], &texProgram));

					spriteexp[j]->setNumberAnimations(1);
					spriteexp[j]->setAnimationSpeed(0, 15);
					spriteexp[j]->addKeyframe(0, glm::vec2(0.f, 0.75f));
					spriteexp[j]->addKeyframe(0, glm::vec2(0.25f, 0.75f));
					spriteexp[j]->addKeyframe(0, glm::vec2(0.5f, 0.75f));
					spriteexp[j]->addKeyframe(0, glm::vec2(0.75f, 0.75f));
					spriteexp[j]->changeAnimation(0);
					spriteexp[j]->setPosition(glm::vec2(post.x+15, post.y+15));
					contexp.push_back(0);
				}
				for (int w = i; w < burbujas.size() - 1; ++w) { burbujas[w] = burbujas[w + 1]; }
				burbujas.pop_back();
				if (mode != 4) {
					burbujas.push_back(new Bubble());
					int j = burbujas.size() - 1;
					burbujas[j]->init(glm::ivec2(0, 0), texProgram, mode + 1, false, false, false, false, posp.y + hact / 2 - hsig / 2, currentLevel);
					burbujas[j]->setPosition(glm::vec2(posp.x, posp.y + hact / 2 - hsig / 2));
					burbujas[j]->setTileMap(map);

					burbujas.push_back(new Bubble());
					j = burbujas.size() - 1;
					burbujas[j]->init(glm::ivec2(0, 0), texProgram, mode + 1, true, false, false, false, posp.y + hact / 2 - hsig / 2, currentLevel);
					burbujas[j]->setPosition(glm::vec2(posp.x + wact - wsig, posp.y + hact / 2 - hsig / 2));
					burbujas[j]->setTileMap(map);

				}

				if (dinamita && contdin == 20) contdin = 0;


			}
		}
	}

}

void Scene::destroyPlatform(int x, int y)
{
	for (int i = 0; i < platforms.size(); ++i) {
		if (platforms[i]->posy/8 == y && platforms[i]->posx/8 <= x && platforms[i]->posx/8 +3 >= x) {
			points += 500;
			platforms[i]->sprite->changeAnimation(1);
		}

	}	
	mciSendString(L"stop glass", NULL, 0, NULL);
	mciSendString(L"open \"sound/glass.wav\" type mpegvideo alias glass", NULL, 0, NULL);
	mciSendString(L"seek glass to start", NULL, 0, NULL);
	mciSendString(L"play glass", NULL, 0, NULL);

	
}