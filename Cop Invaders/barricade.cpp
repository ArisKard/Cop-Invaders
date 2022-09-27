#include "barricade.h"
#include "graphics.h"
#include "config.h"
#include "util.h"
#include "Game.h"

void Barricade::update()
{
}

void Barricade::init()
{
}

void Barricade::draw()
{
	graphics::setOrientation(rotation);
	graphics::Brush br;
	br.texture = std::string(ASSET_PATH) + "car.png";
	br.outline_opacity = 0.0f;
	graphics::drawRect(pos_x, pos_y, size, size - 50.0f, br);
	graphics::resetPose();

	br.texture = std::string(ASSET_PATH) + "flame2.png";
	br.fill_opacity = 0.6f + 0.25*sinf(graphics::getGlobalTime() / 300);
	graphics::drawRect(pos_x, pos_y - 15.0f, size, size, br);

	br.outline_opacity = 0.0f;
	br.fill_color[0] = 1.0f;
	br.fill_color[1] = 0.0f;
	br.fill_color[2] = 0.0f;
	br.texture = "";
	br.fill_secondary_color[0] = 1.0f * (1.0f - bar_life) + bar_life * 0.2f;
	br.fill_secondary_color[1] = 0.2f;
	br.fill_secondary_color[2] = 0.2f * (1.0f - bar_life) + bar_life * 1.0f;
	br.gradient = true;
	br.gradient_dir_u = 1.0f;
	br.gradient_dir_v = 0.0f;

	graphics::drawRect(pos_x - ((1.0f - bar_life) * size / 4), pos_y + size/2 - 10, bar_life * (size/2), 5, br);

	br.outline_opacity = 1.0f;
	br.gradient = false;
	br.fill_opacity = 0.0f;
	graphics::drawRect(pos_x, pos_y + size/2 - 10, size / 2, 5, br);
}

void Barricade::init(unsigned int i)
{
	pos_x = (CANVAS_WIDTH / 8 - 15.0f) + i * 175.0f;
	pos_y = CANVAS_HEIGHT - 140.0;
	size = 120.0f;
	rotation = 195.0f;
}

Barricade::Barricade(const Game& mygame, unsigned int i)
	: GameObject(mygame)
{
	init(i);
}

Barricade::~Barricade()
{
}

Disk Barricade::getCollisionHull() const
{
	Disk disk;
	disk.cx = pos_x;
	disk.cy = pos_y;
	disk.radius = size * 0.42f;
	return disk;
}
