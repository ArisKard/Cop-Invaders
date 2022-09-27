#include "life.h"

void Life::update()
{
}

void Life::draw()
{
	br.outline_opacity = 0.0f;
	br.texture = std::string(ASSET_PATH) + "life.png";
	graphics::drawRect(pos_x, pos_y, size, size, br);
}

void Life::init()
{
}

Life::Life(const Game& mygame, float x)
	: GameObject(mygame)
{
	pos_x = CANVAS_WIDTH - x;
}

Life::~Life()
{
}
