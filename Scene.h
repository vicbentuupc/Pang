#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Rope.h"
#include "Bubble.h"
#include "Platform.h"
#include "PowerUp.h"


// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene
{

public:
	Scene();
	~Scene();

	void init();
	void startLevel3(int lives);
	void update(int deltaTime);
	void render();
	bool createRope(glm::ivec2 pos);
	void destroyPlatform(int x, int y);
	void preLoadLevel(int nlives);
	void startLevel2(int lives);
	void passLevel();
	void loadMap();
	void startLevel1(int lives);
	void death();
	void startLevel(int x, int lives);
	void tutorial();
	void credits();

	int passLevelTimer;
	int deathTimer;
	int lives;
	int deathRagdollTimer = -777;
	int currentLevel;
	bool godMode = false;
	int godModeBlinkTimer = 200;
	bool visible = true;
	bool rope2 = false;
	bool dinamita = false;
	bool parat = false;
	int godCounter = 0;
	int timer = -777;
	int stageClearedTimer = -777;
	int finalTimer = -777;
	bool hurrymusic = true;
	int finalScore = -777;

	bool tutocredits = false;
	
	

private:
	void initShaders();

private:
	TileMap *map;
	Player *player;

	vector<Rope*> rope;
	vector<Texture*> spritesheetExplosionRope;
	vector<Sprite*> explosionRope;
	vector<bool> petas;

	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;
	vector<Bubble*> burbujas;
	vector<Sprite*> spriteexp;
	vector<Texture*> spritesheetexp;
	vector<int> contexp;
	int cont = 0;
	
	vector<Sprite*> spritepts;
	vector<Texture*> spritesheetpts;
	vector<int> contpts;

	void updateBurbujas(int deltaTime);
	
	vector<Platform*> platforms;
	bool circleRect(int cx, int cy, float radius, float rx, float ry, float rw, float rh)const;

	vector<PowerUp*> powerup;
	vector<int> contpowerup;

	PowerUp* comida;
	int contcomida = 0;

	vector <int> ptsbb = {0,50,100,150,200};
	int multbb = 2;
	int lastpet = 0;

	int points = 0;


	bool muerto = false;
	bool muertoder =  true;
	
	int contdin = 19;

	
	int contparat = 0;

	Texture* defaultSpritesheet;
	Sprite* defaultSprite = NULL;

	int vidFrame = 0;
	vector<Texture*> frameTextures;
	int mapSelector = -1;
	int clickCd = 0;
	Texture* defaultSpritesheet2;
	Sprite* defaultSprite2 = NULL;
	Texture* readySpritesheet;
	Sprite* readySprite;

	Texture* wallpaperSpritesheet;
	Sprite* wallpaperSprite;

	Texture* deathWallSpritesheet;
	Sprite* deathWallSprite;

	Bubble* deadPlayer;
	Texture* deadPlayerSpritesheet;


	Texture* blackTexture;
	Sprite* blackSprite;
	Texture* livesTexture;
	Sprite* livesSprite;
	Texture* cityTexture;
	Sprite* citySprite;
	Texture* scoreTexture;
	Sprite* scoreSprite;

	Texture* timerTexture;
	Sprite* timerSprite;
	Texture* timerLettersTexture;
	Sprite* timerLettersSprite;

	Texture* stageClearedTexture;
	Sprite* stageClearedSprite;

	Texture* finalScoreTexture;
	Sprite* finalScoreSprite;

	Texture* credtutoTexture;
	Sprite* credtutoSprite;
	



};


#endif // _SCENE_INCLUDE

