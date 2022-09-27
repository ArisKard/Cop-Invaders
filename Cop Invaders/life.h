#pragma once
#include "gameobject.h"
#include "config.h"
#include "graphics.h"

class Life : public GameObject
{
	float pos_x;
	const float pos_y = 15.0f;
	const float size = 30.0f;

public:
	void update() override;
	void draw() override;
	void init() override;
	Life(const class Game& mygame, float x);
	~Life();
};