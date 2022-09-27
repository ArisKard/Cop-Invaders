#pragma once
#include "util.h"
#include "graphics.h"

class Collidable
{
public:
	virtual Disk getCollisionHull() const = 0;
};


class GameObject
{
protected:	
	const class Game& game;
	graphics::Brush br;
	float pos_x;
	float pos_y;
	float size;
	float rotation;
	float speed;
public:
	GameObject(const class Game & mygame);
	virtual void update() = 0;
	virtual void draw() = 0;
	virtual void init() = 0;
};