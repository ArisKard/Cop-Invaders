#pragma once
#include "gameobject.h"
#include "graphics.h"
#include "config.h"

class Barricade : public GameObject, public Collidable
{
	float bar_life = 1.0f;
	bool active = true;
	
public:
	void update() override;
	void draw() override;
	void init() override;
	void init(unsigned int i);
	bool isActive() { return active; }
	void setPosX(float x) { pos_x = x; }
	void reduceLife() { bar_life -= 0.06f; }
	float getLife() const { return bar_life; }
	float getPosX() const { return pos_x; }
	float getPosY() const { return pos_y; }
	Barricade(const class Game& mygame, unsigned int i);
	~Barricade();
	Disk getCollisionHull() const override;
};