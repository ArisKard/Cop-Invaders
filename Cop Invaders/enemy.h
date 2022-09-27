#pragma once
#include "gameobject.h"
#include "config.h"
#include "graphics.h"

class Enemy : public GameObject, public Collidable
{
	float collision = 0.0f;
	float shoot = 0.0f;
	float move = 1.0;
	float move_time = 0.0f;
	unsigned int column, row, max_column, min_column;
	bool active = true;
	bool left_to_right = true;

public:
	void update() override {};
	void update(unsigned int max_column, unsigned int min_column);
	void draw() override;
	void init() override {};
	void init(unsigned int i, unsigned int j);
	bool isActive() { return active; }
	float getPosX() { return pos_x; }
	float getPosY() { return pos_y; }
	void changeCollision() { collision = 1.0f; }
	float getCollision() const { return collision; }
	void changeSpeed(float speedup) { speed += speedup; }
	void changeShoot() { shoot = 1.0f - shoot; }
	float getShoot() const { return shoot; }
	Enemy(const Game& mygame, unsigned int i, unsigned int j);
	~Enemy();
	Disk getCollisionHull() const override;
};