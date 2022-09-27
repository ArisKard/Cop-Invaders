#pragma once
#include "player.h"
#include "enemy.h"
#include "projectile.h"
#include "bullet.h"
#include "life.h"
#include "barricade.h"
#include "bonusenemy.h"

class Game
{
	typedef enum {STATUS_START, STATUS_PLAYING, STATUS_GAMEOVER, STATUS_PAUSED} status_t;
	status_t status = STATUS_START;

	float fire_time;
	float glow_time;
	float proj_collision_time;
	float dead_time;
	float game_drawn_time;
	float bullet_times[12];
	float bullet_time;
	float paused_time;
	float unpaused_time;
	float bonusenemy_spawntime = 0.0f;
	float speedup;
	float base_bullet_time;
	int score = 0;
	int alive_enemies = 60;
	unsigned int rows[12];
	unsigned int max_column = 11;
	unsigned int min_column = 0;
	unsigned int num_of_lives = 3;
	unsigned int num_of_barricades = 5;
	bool game_over = false;
	bool bonus_ltr = true;
	bool collision = false;
	bool column_destroyed = false;
	bool player_initialized = false;
	bool pause = true;

	Player* player = nullptr;
	Enemy* enemy = nullptr;
	Projectile* projectile = nullptr;
	Bullet* bullet = nullptr;
	BonusEnemy* bonusenemy = nullptr;

	Life* life[3];
	Barricade* bar[5];
	Enemy* enemyArray[5][12];
	Bullet* bulletArray[12];
	
	void updateStartScreen();
	void updateLevelScreen();
	void updatePausedScreen();
	void updateGameOverScreen();
	void drawStartScreen();
	void drawLevelScreen();
	void drawPausedScreen();
	void drawGameOverScreen();

	void checkCollision();
	void checkPlayerCollision();
	void checkProjectileCollision();
	void checkBarrProjCollision();
	void checkBarrBulletCollision();
	void checkBonusEnemyCollision();
	void checkPause();
	void checkProjectile();
	void spawnProjectile();
	void checkBullet();
	void spawnBullet();
	void checkBonusEnemy();
	void spawnBonusEnemy();
	void deleteProjectile();
	void playerDead();
	void enemyClose();
	void notGlow();
	void deleteBonusEnemy();
	void speedUp();
	void reFormation();
	void allEnemiesDead();

public:

	void update();
	void draw();
	void init();
	Game();
	~Game();
};