#include "player.h"
#include "graphics.h"
#include "util.h"

Player::Player(const Game& mygame)
	: GameObject(mygame)
{
	init();
}

Player::~Player()
{

}


void Player::update()
{
	if (graphics::getKeyState(graphics::SCANCODE_A))
	{
		pos_x -= speed * graphics::getDeltaTime() / 20.0f;
	}
	if (graphics::getKeyState(graphics::SCANCODE_D))
	{
		pos_x += speed * graphics::getDeltaTime() / 20.0f;
	}
	
	if (pos_x < 0) pos_x = 0;
	if (pos_x > CANVAS_WIDTH) pos_x = CANVAS_WIDTH;
	if (pos_y < 0) pos_y = 0;
	if (pos_y > CANVAS_HEIGHT) pos_y = CANVAS_HEIGHT;
}

void Player::draw()
{

	if (plglow)
	{
		glow = sinf(graphics::getGlobalTime() / 50);
	}

	graphics::setOrientation(rotation);
	br.texture = std::string(ASSET_PATH) + "player.png";
	br.outline_opacity = 0.0f;
	br.fill_opacity = glow * 1.0f;
	graphics::drawRect(pos_x, pos_y, 30, 30, br);
	

	br.texture = std::string(ASSET_PATH) + "mol.png";
	br.outline_opacity = 0.0f;
	br.fill_opacity = glow * 0.6f;
	graphics::drawRect(pos_x + 19, pos_y, 20, 20, br);
	graphics::resetPose();
}


void Player::init()
{
	speed = 8.0f;
	pos_x = CANVAS_WIDTH / 2;
	pos_y = CANVAS_HEIGHT - 20;
	rotation = 0.0f;
}

Disk Player::getCollisionHull() const
{
	Disk disk;
	disk.cx = pos_x;
	disk.cy = pos_y;
	disk.radius = 30*0.5f;
	return disk;
}
