#include "projectile.h"
#include "graphics.h"
#include "config.h"
#include "util.h"
#include "Game.h"

void Projectile::update()
{	
	pos_y -= speed * graphics::getDeltaTime();
	rotation -= 0.4f * graphics::getDeltaTime();
	rotation = fmodf(rotation, 360);
	

	if (pos_y < - size)
	{
		active = false;
	}
}

void Projectile::draw()
{
	graphics::setOrientation(rotation);
	br.texture = std::string(ASSET_PATH) + "mol.png";
	br.outline_opacity = 0.0f;
	br.fill_opacity = 1.0f - proj_collision;
	graphics::drawRect(pos_x + 19, pos_y, size, size, br);
	graphics::resetPose();

	br.texture = std::string(ASSET_PATH) + "explosion.png";
	br.fill_opacity = proj_collision;
	graphics::drawRect(pos_x + 19, pos_y, size + 20.0f, size + 20.0f, br);
}


void Projectile::init()
{
	speed = 0.5f;
	rotation = 30.0f;
	size = 20.0f;
}

Projectile::Projectile(const Game& mygame)
	: GameObject(mygame)
{
	init();
}

Projectile::~Projectile()
{
}

Disk Projectile::getCollisionHull() const
{
	Disk disk;
	disk.cx = pos_x + 19;
	disk.cy = pos_y;
	disk.radius = size * 0.55f;
	return disk;
}
