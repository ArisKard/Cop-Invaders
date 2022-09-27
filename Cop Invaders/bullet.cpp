#include "bullet.h"
#include "graphics.h"
#include "config.h"
#include "util.h"
#include "Game.h"

void Bullet::update()
{
	pos_y += speed * graphics::getDeltaTime();

	if (pos_y > CANVAS_HEIGHT + size)
	{
		active = false;
	}
}

void Bullet::draw()
{
	graphics::setOrientation(rotation);
	br.texture = std::string(ASSET_PATH) + "bullet.png";
	br.outline_opacity = 0.0f;
	graphics::drawRect(pos_x, pos_y, size, size, br);
	graphics::resetPose();

}


void Bullet::init()
{
	speed = 0.4f;
	rotation = -43.0f;
	size = 20.0f;
}

Bullet::Bullet(const Game& mygame)
	: GameObject(mygame)
{
	init();
}

Bullet::~Bullet()
{
}

Disk Bullet::getCollisionHull() const
{
	Disk disk;
	disk.cx = pos_x;
	disk.cy = pos_y;
	disk.radius = size * 0.55f;
	return disk;
}
