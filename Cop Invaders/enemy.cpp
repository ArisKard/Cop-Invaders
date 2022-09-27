#include "enemy.h"
#include "graphics.h"
#include "config.h"
#include "util.h"
#include "Game.h"

void Enemy::update(unsigned int max_column, unsigned int min_column)
{
	
	if (graphics::getGlobalTime() > move_time + (1000.0f - speed*5000.0f))
	{
		move_time = graphics::getGlobalTime();
		move = (1.0 - move);
	}

	if (left_to_right)
	{
		pos_x += speed * graphics::getDeltaTime();
		if ((float)pos_x + size / 2 > CANVAS_WIDTH - 60.0f*(max_column-column))
		{
			pos_y += 13.0f;
			left_to_right = false;
		}
	}
	else
	{
		pos_x -= speed * graphics::getDeltaTime();
		if ((float)pos_x - size / 2 - 60.0f*(abs((int)column - (int)min_column)) < 0.0f)
		{
			pos_y += 13.0f;
			left_to_right = true;
		}
	}

	if (pos_y > CANVAS_HEIGHT + size)
	{
		active = false;
	}
}

void Enemy::draw()
{
	
	br.texture = std::string(ASSET_PATH) + "pig1.png";
	br.fill_opacity = 1.0f*move*(1-shoot);
	graphics::drawRect(pos_x, pos_y, size, size, br);
	
	br.texture = std::string(ASSET_PATH) + "pig2.png";
	br.fill_opacity = 1.0f*(1-move)*(1-shoot);
	graphics::drawRect(pos_x, pos_y, size, size, br);

	br.texture = std::string(ASSET_PATH) + "flame.png";
	br.fill_opacity = collision*1.0f;
	graphics::drawRect(pos_x, pos_y, size, size, br);
	
	br.texture = std::string(ASSET_PATH) + "pigshoot.png";
	br.fill_opacity = shoot * 1.0f;
	graphics::drawRect(pos_x, pos_y, size, size, br);
}

void Enemy::init(unsigned int i, unsigned int j)
{
	speed = 0.025f;
	size = 45.0f;
	pos_x = 85.0f + 60.0f * j;
	pos_y = 50.0f + 50.0 * i;
	row = i;
	column = j;

	br.outline_opacity = 0.0f;
}

Enemy::Enemy(const Game& mygame, unsigned int i, unsigned int j)
	 : GameObject(mygame)
{
	init(i, j);
}

Enemy::~Enemy()
{
}

Disk Enemy::getCollisionHull() const
{
	Disk disk;
	disk.cx = pos_x;
	disk.cy = pos_y;
	disk.radius = size * 0.45f;
	return disk;
}
