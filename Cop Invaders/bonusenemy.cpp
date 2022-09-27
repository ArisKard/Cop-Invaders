#include "bonusenemy.h"
#include "graphics.h"
#include "config.h"
#include "util.h"
#include "Game.h"

void BonusEnemy::update()
{
	if (left_to_right)
	{
		pos_x -= speed * graphics::getDeltaTime();

		if (pos_x < -size)
		{
			active = false;
		}
	}
	else
	{
		pos_x += speed * graphics::getDeltaTime();

		if (pos_x > CANVAS_WIDTH + size)
		{
			active = false;
		}
	}
}

void BonusEnemy::draw()
{
	graphics::setOrientation(rotation);
	br.texture = std::string(ASSET_PATH) + "pigcar.png";
	br.fill_opacity = 1.0f;
	graphics::drawRect(pos_x, pos_y, size, size - 33.0f, br);
	graphics::resetPose();

	br.texture = std::string(ASSET_PATH) + "bonusexplosion.png";
	br.fill_opacity = collision * 1.0f;
	graphics::drawRect(pos_x, pos_y, size, size, br);
}

void BonusEnemy::init(bool ltr)
{
	pos_x = CANVAS_WIDTH + 40.0f;
	pos_y = 30.0f;
	speed = 0.2f;
	size = 65.0f;
	left_to_right = ltr;
	rotation = 0.0f;

	if (!left_to_right)
	{
		pos_x = -size;
		rotation = -180.0f;
	}

	br.outline_opacity = 0.0f;
}

BonusEnemy::BonusEnemy(const Game& mygame, bool ltr)
	: GameObject(mygame)
{
	init(ltr);
}

BonusEnemy::~BonusEnemy()
{
}

Disk BonusEnemy::getCollisionHull() const
{
	Disk disk;
	disk.cx = pos_x;
	disk.cy = pos_y;
	disk.radius = size * 0.5f;
	return disk;
}
