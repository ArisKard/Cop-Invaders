#pragma once
#include "gameobject.h"
#include "config.h"
#include "graphics.h"
#include "projectile.h"

class Player : public GameObject, public Collidable
{
	float glow = 1.0f;
	bool plglow = false;

public:
	void update() override;
	void draw() override;
	void init() override;
	float getPosX() { return pos_x; }
	float getPosY() { return pos_y; }
	float getPosX() const { return pos_x; }
	bool getGlow() const { return glow; }
	float getRotation() const { return rotation; }
	void playerGlow() { plglow = true; }
	void playerNotGlow() { plglow = false; glow = 1.0f; }
	void playerDied() { rotation = -90.0f; }
	Player(const class Game& mygame);
	~Player();
	Disk getCollisionHull() const override;
};