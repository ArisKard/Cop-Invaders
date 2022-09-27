#include "Game.h"
#include "graphics.h"
#include "config.h"

//Checks if Bonus Enemy is active and if not, it gets deleted.
void Game::checkBonusEnemy()
{
	if (bonusenemy && !(bonusenemy->isActive()))
	{
		delete bonusenemy;
		bonusenemy = nullptr;
	}
}

//Spawns Bonus Enemy ever 10 seconds.
void Game::spawnBonusEnemy()
{
	float time = graphics::getGlobalTime();
	if (!bonusenemy && time - bonusenemy_spawntime > 10000.0f && time > game_drawn_time + 6000.0f)
	{
		graphics::playSound(std::string(ASSET_PATH) + "bonusenemy.mp3", 0.5f, false);
		bonusenemy_spawntime = graphics::getGlobalTime();
		bonusenemy = new BonusEnemy(*this, bonus_ltr);
		bonus_ltr = !bonus_ltr;
	}
}

//Checks if Molotov is active and if not, it gets deleted.
void Game::checkProjectile()
{
	if (projectile && !projectile->isActive())
	{
		delete projectile;
		projectile = nullptr;
	}
}

//Spawns Molotov, if there's not any and if Key State is SPACE.
void Game::spawnProjectile()
{
	if (!projectile && graphics::getKeyState(graphics::SCANCODE_SPACE))
	{
		graphics::playSound(std::string(ASSET_PATH) + "throw.mp3", 0.4f, false);
		projectile = new Projectile(*this);
		projectile->setPosX(player->getPosX());
		projectile->setPosY(player->getPosY());
	}
}

//Checks if Bullet of column j is active. If not, it gets deleted.
void Game::checkBullet()
{
	for (int j = 0; j < 12; j++)
	{
		if (bulletArray[j] && !bulletArray[j]->isActive())
		{
			delete bulletArray[j];
			bulletArray[j] = nullptr;

			if (enemyArray[0][j])
				enemyArray[0][j]->changeShoot();
		}
	}
}

/*Spawns a Bullet of column j randomly, if player's X position is within 300 Canvas Units from column j. The Bullets spawn
every base_bullet_time + (alive_enemies * 10) seconds, depending on User's choice of difficulsty.*/
void Game::spawnBullet()
{
	if (enemyArray)
	{
		int j = rand0to12();

		if (enemyArray[0][j] && !bulletArray[j] && abs(enemyArray[0][j]->getPosX() - player->getPosX()) < 300.0f && graphics::getGlobalTime() - bullet_time > base_bullet_time + (alive_enemies*10.0f) && graphics::getGlobalTime() - game_drawn_time > 5000.0f)
		{
			graphics::playSound(std::string(ASSET_PATH) + "laser.wav", 0.2f, false);
			bullet_time = graphics::getGlobalTime();
			bulletArray[j] = new Bullet(*this);
			bulletArray[j]->setPosX(enemyArray[0][j]->getPosX());
			bulletArray[j]->setPosY(enemyArray[0][j]->getPosY());
			enemyArray[0][j]->changeShoot();
		}
	}
}

/*It checks if there's a collision between a Molotov and an Enemy. If Enemy gets hit, it deletes the Molotov,
reduces the number of alive Enemies and holds a point in time so that the fire is displayed. If the number
of enemies is 0, it holds a point in time so that the Game Over Screen is not immediately displayed.*/
void Game::checkCollision()
{
	if (!projectile || !enemyArray)
	{
		return;
	}

	if (enemyArray)
	{
		Disk d1 = projectile->getCollisionHull();
		
		for (int i = 0; i < 5; i++)
		{
			for (int j = 0; j < 12; j++)
			{
				if (enemyArray[i][j]!=nullptr)
				{
					Disk d2 = enemyArray[i][j]->getCollisionHull();

					float dx = d1.cx - d2.cx;
					float dy = d1.cy - d2.cy;

					if (sqrt(dx * dx + dy * dy) < d1.radius + d2.radius)
					{
						enemyArray[i][j]->changeCollision();
						graphics::playSound(std::string(ASSET_PATH) + "enemyscream.wav", 0.8f, false);
						graphics::playSound(std::string(ASSET_PATH) + "enemyhit.wav", 1.0f, false);
						fire_time = graphics::getGlobalTime();
						delete projectile;
						projectile = nullptr;
						collision = true;
						alive_enemies -= 1;

						if (alive_enemies == 0)
							dead_time = graphics::getGlobalTime();

						return;
					}
				}
			}
		}
	}	
}

/*It checks if the Player gets Hit from a Bullet. If it does, the Bullet gets deleted, the number of Lives
gets reduced, and the Player glows. If the nuumber of lives is 0, the Player dies.*/
void Game::checkPlayerCollision() 
{
	if (!bulletArray || !player || num_of_lives == 0)
	{
		return;
	}

	Disk d1 = player->getCollisionHull();
	
	for (int j = 0; j < 12; j++)
	{
		if (bulletArray[j] != nullptr)
		{
			Disk d2 = bulletArray[j]->getCollisionHull();

			float dx = d1.cx - d2.cx;
			float dy = d1.cy - d2.cy;

			if (sqrt(dx * dx + dy * dy) < d1.radius + d2.radius)
			{
				graphics::playSound(std::string(ASSET_PATH) + "damage.mp3", 0.3f, false);
				delete bulletArray[j];
				bulletArray[j] = nullptr;
				num_of_lives -= 1;
				player->playerGlow();
				glow_time = graphics::getGlobalTime();
				delete life[num_of_lives];
				life[num_of_lives] = nullptr;

				if (enemyArray[0][j])
					enemyArray[0][j]->changeShoot();

				if (num_of_lives == 0)
				{
					graphics::playSound(std::string(ASSET_PATH) + "fall.mp3", 0.9f, false);
					player->playerNotGlow();
					player->playerDied();
					dead_time = graphics::getGlobalTime();
				}

				return;

			}
		}
	}

}

/*Checks if there is a Collision between a Bullet and a Molotov. If yes, Bullet of column j gets deleted
and it holds a point in time so that the explosion is displayed.*/
void Game::checkProjectileCollision()
{
	if (!bulletArray || !projectile)
	{
		return;
	}

	Disk d1 = projectile->getCollisionHull();
	
	for (int j = 0; j < 12; j++)
	{
		if (bulletArray[j] != nullptr)
		{
			Disk d2 = bulletArray[j]->getCollisionHull();

			float dx = d1.cx - d2.cx;
			float dy = d1.cy - d2.cy;

			if (sqrt(dx * dx + dy * dy) < d1.radius + d2.radius)
			{
				delete bulletArray[j];
				bulletArray[j] = nullptr;
				score += 5;
				graphics::playSound(std::string(ASSET_PATH) + "explode.wav", 0.8f, false);
				proj_collision_time = graphics::getGlobalTime();
				projectile->changeProjCollision();
				projectile->setSpeedto0();

				if (enemyArray[0][j])
					enemyArray[0][j]->changeShoot();
				
				return;
			}
		}
	}
}

/*Checks if there's a collision between a Barricade and a Molotcv. If it does, the life of the Barricade
gets reduced, the Molotov gets deleted and if Barricade's life is 0, it gets deleted also.*/
void Game::checkBarrProjCollision() 
{
	if (!bar || !projectile || num_of_barricades==0)
	{
		return;
	}
	
	Disk d1 = projectile->getCollisionHull();
	for (int i = 0; i < 5; i++)
	{
		if (bar[i] != nullptr)
		{
			Disk d = bar[i]->getCollisionHull();
			float dx = d1.cx - d.cx;
			float dy = d1.cy - d.cy;
			if (sqrt(dx * dx + dy * dy) < d.radius + d1.radius)
			{
				graphics::playSound(std::string(ASSET_PATH) + "barr.wav", 0.1f, false);

				delete projectile;
				projectile = nullptr;
				bar[i]->reduceLife();
				if (bar[i]->getLife() <= 0.00001f)
				{
					graphics::playSound(std::string(ASSET_PATH) + "impact.mp3", 0.9f, false);
					delete bar[i];
					bar[i] = nullptr;
					num_of_barricades -= 1;
				}
			}
		}
	}	
}

/*Checks if there's a collision between a Barricade and a Bullet. If it does, the life of the Barricade
gets reduced, Bullet of column j gets deleted and if Barricade's life is 0, it gets deleted also.*/
void Game::checkBarrBulletCollision()
{
	if (!bar || !bulletArray || num_of_barricades == 0)
	{
		return;
	}

	for (int i = 0; i < 5; i++)
	{
		if (bar[i] != nullptr)
		{
			Disk d = bar[i]->getCollisionHull();

			for (int j = 0; j < 12; j++)
			{
				if (bulletArray[j] != nullptr)
				{
					Disk d1 = bulletArray[j]->getCollisionHull();

					float dx = d1.cx - d.cx;
					float dy = d1.cy - d.cy;
					if (sqrt(dx * dx + dy * dy) < d.radius + d1.radius)
					{
						graphics::playSound(std::string(ASSET_PATH) + "barr.wav", 0.2f, false);
						delete bulletArray[j];
						bulletArray[j] = nullptr;

						if (enemyArray[0][j])
							enemyArray[0][j]->changeShoot();

						bar[i]->reduceLife();
						if (bar[i]->getLife() <= 0.0f + 0.00001f)
						{
							graphics::playSound(std::string(ASSET_PATH) + "impact.mp3", 0.7f, false);
							delete bar[i];
							bar[i] = nullptr;
							num_of_barricades -= 1;
						}
					}
				}
			}
		}
	}
}

/*Checks if there's the Bonus Enemy gets hit. If it does, the Molotov gets deleted and it holds
a point in time so that the explosion gets displayed.*/
void Game::checkBonusEnemyCollision()
{
	if (!bonusenemy || !projectile)
	{
		return;
	}
	
	Disk d1 = projectile->getCollisionHull();
	Disk d2 = bonusenemy->getCollisionHull();
	
	float dx = d1.cx - d2.cx;
	float dy = d1.cy - d2.cy;

	if (sqrt(dx * dx + dy * dy) < d1.radius + d2.radius)
	{
		delete projectile;
		projectile = nullptr;
		score += 200;
		graphics::playSound(std::string(ASSET_PATH) + "bonusenexplode.wav", 0.6f, false);
		bonusenemy->changeCollision();
		graphics::playSound(std::string(ASSET_PATH) + "bonusenscream.wav", 0.8f, false);
		//graphics::playSound(std::string(ASSET_PATH) + "enemyhit.wav", 1.0f, false);
		fire_time = graphics::getGlobalTime();
	}
}

//If Molotov - Bullet collision time is over 0.4 seconds, Molotov gets deleted.
void Game::deleteProjectile()
{
	if (projectile)
	{
		if (projectile->getProjCollision() > 0.0f && (graphics::getGlobalTime() - 400.0 > proj_collision_time))
		{
			delete projectile;
			projectile = nullptr;
		}
	}
}

//If Player dead for over than 2 seconds, go to Game Over Screen.
void Game::playerDead()
{
	if (player)
	{
		if ((player->getRotation() == -90.0f) && (graphics::getGlobalTime() - 2000.0 > dead_time))
		{
			status = STATUS_GAMEOVER;
		}
	}
}

/*Checks if there is any Enemy close than 45 Canvas Units to Player. If yes, go to Game Over Screen.
This gets checked after 50 seconds of Play Time, cause it's impossible for an Enemy to get so close earlier.*/
void Game::enemyClose()
{
	if (graphics::getGlobalTime() > 50000.0f)
	{
		for (int i = 4; i >= 0; i--)
		{
			for (int j = 0; j < 12; j++)
			{
				if (enemyArray[i][j] && player->getPosY() - enemyArray[i][j]->getPosY() < 45.0f)
				{
					status = STATUS_GAMEOVER;
				}
			}
		}
	}
}

//If Player is glowing for over than 1.5 seconds, stop glowing.
void Game::notGlow()
{
	if (player)
	{
		if ((player->getGlow()) && (graphics::getGlobalTime() - 1500.0 > glow_time))
		{
			player->playerNotGlow();
		}
	}
}

//If Bonus Enemy got hit over 0.75 seconds ago, delete it.
void Game::deleteBonusEnemy()
{
	if (bonusenemy && (bonusenemy->getCollision() > 0.0f) && (graphics::getGlobalTime() - 750.0 > fire_time))
	{
		delete bonusenemy;
		bonusenemy = nullptr;
	}

}

//If an Enemy got hit, all Enemies get a Speed Up, according to User's original choice of difficulty.
void Game::speedUp()
{
	if (collision)
	{
		for (int i = 0; i < 5; i++)
		{
	 		for (int j = 0; j < 12; j++)
			{
				if (enemyArray[i][j])
				{
					enemyArray[i][j]->changeSpeed(speedup);
				}
			}
		}	
		collision = false;
	}
}

/*If an Enemy got hit over than 0.3 seconds ago, it gets deleted. In order to maintain the Enemy formation,
the Array rows[j] holds the number of rows each column has left. If it reaches 0 and the column was the rightmost
one (max column), it searches the formation for the next rightmost active column. The same is done for the
leftmost columns (min column).*/
void Game::reFormation()
{
	if (enemyArray)
	{
		for (int i = 0; i < 5; i++)
		{
			for (int j = 0; j < 12; j++)
			{
				if (enemyArray[i][j] != nullptr)
				{
					if ((enemyArray[i][j]->getCollision() > 0.0f) && (graphics::getGlobalTime() - 300.0 > fire_time))
					{
						delete enemyArray[i][j];
						enemyArray[i][j] = nullptr;
						score += (5 - i) * 10;

						rows[j] -= 1;

						if (rows[j] == 0 && j == max_column && max_column != 0 && alive_enemies > 0)
						{
							do
							{
								max_column -= 1;
							} while (rows[max_column] == (unsigned int)0);
							return;
						}

						if (rows[j] == 0 && j == min_column && j != 9 && alive_enemies > 0)
						{
							do
							{
								min_column += 1;
							} while (rows[min_column] == (unsigned int)0);
							return;
						}
					}
				}
			}
		}
	}
}

void Game::allEnemiesDead()
{
	if (alive_enemies == 0 && (graphics::getGlobalTime() - 2000.0 > dead_time))
	{
		status = STATUS_GAMEOVER;
		//return;
	}
}

//If Key State is S, the Game is Paused. Cannot Pause again withing 0.5 seconds.
void Game::checkPause()
{ 
	if (graphics::getKeyState(graphics::SCANCODE_S) && graphics::getGlobalTime() - 500.0f > unpaused_time)
	{
		graphics::playSound(std::string(ASSET_PATH) + "pause.wav", 0.5f, false);
		paused_time = graphics::getGlobalTime();
		status = STATUS_PAUSED;
	}	   
}


//------------ U p d a t e    S c r e e n s --------------//


//If Key State is Space, the Game starts.
void Game::updateStartScreen()
{
	if(graphics::getKeyState(graphics::SCANCODE_1))
	{
		game_drawn_time = graphics::getGlobalTime();
		status = STATUS_PLAYING;
		speedup = 0.0017f;
		base_bullet_time = 200.0f;
	}
	else if (graphics::getKeyState(graphics::SCANCODE_2))
	{
		game_drawn_time = graphics::getGlobalTime();
		status = STATUS_PLAYING;
		speedup = 0.0023f;
		base_bullet_time = 100.0f;
	}
	else if (graphics::getKeyState(graphics::SCANCODE_3))
	{
		game_drawn_time = graphics::getGlobalTime();
		status = STATUS_PLAYING;
		speedup = 0.0030f;
		base_bullet_time = 0.0f;
	}
}

//All In-Game Updates.
void Game::updateLevelScreen()
{

	if (!player_initialized && graphics::getGlobalTime() > 2000)
	{
		player = new Player(*this);
		player_initialized = true;
	}

	checkProjectile();
	spawnProjectile();
	checkBullet();
	spawnBullet();
	checkBonusEnemy();
	spawnBonusEnemy();
	checkCollision();
	checkPlayerCollision();
	checkProjectileCollision();
	checkBarrBulletCollision();
	checkBarrProjCollision();
	checkBonusEnemyCollision();
	deleteProjectile();
	allEnemiesDead();
	playerDead();
	enemyClose();
	notGlow();
	deleteBonusEnemy();
	speedUp();
	reFormation();
	checkPause();

	if (player)
		player->update();

	if (bonusenemy)
		bonusenemy->update();
	
	if (projectile)
		projectile->update();

	if (bulletArray)
	{
		for (int j = 0; j < 12; j++)
		{
			if (bulletArray[j] != nullptr)
				bulletArray[j]->update();
		}
	}
		
	if (enemyArray)
	{
		for (int i = 0; i < 5; i++)
		{
			for (int j = 0; j < 12; j++)
			{
				if (enemyArray[i][j] != nullptr)
					enemyArray[i][j]->update(max_column, min_column);
			}
		}
	}
}

//Unpauses the Game and returns to Game Screen. Cannot unpause more than once in less than 0.5 seconds.
void Game::updatePausedScreen()
{
	if (graphics::getKeyState(graphics::SCANCODE_S) && graphics::getGlobalTime() - 500.0f > paused_time)
	{
		graphics::playSound(std::string(ASSET_PATH) + "unpause.wav", 0.5f, false);
		unpaused_time = graphics::getGlobalTime();
		status = STATUS_PLAYING;
	}
}

//Game Over Screen. If the Player won, gives the 1000*number of Lives bonus points.
void Game::updateGameOverScreen() 
{
	if (!game_over)
	{
		graphics::stopMusic(0);
		game_over = true;
		if (alive_enemies > 0)
		{
			graphics::playSound(std::string(ASSET_PATH) + "gameover.mp3", 0.7f, false);
		}
		else
		{
			score += 1000 * num_of_lives;
			graphics::playSound(std::string(ASSET_PATH) + "win.mp3", 0.7f, false);
		}
	}
}


//------------ D r a w    S c r e e n s --------------//


//Draws Start Screen.
void Game::drawStartScreen()
{
	graphics::Brush br;
	char info[40];
	float glow = sinf(graphics::getGlobalTime() / 50);

	br.fill_color[0] = 0.3f;
	br.fill_color[1] = 0.3f;
	br.fill_color[2] = 1.0f;
    br.fill_secondary_color[0] = 1.0;
	br.fill_secondary_color[0] = 0.2;
	br.fill_secondary_color[0] = 0.2;
	br.gradient = true;

	sprintf_s(info, "WELCOME TO COP INVADERS!!!");
	graphics::drawText(20, 100, 70, info, br);

	sprintf_s(info, "points");
	graphics::drawText(CANVAS_WIDTH / 2 + 155, CANVAS_HEIGHT / 2 + 110, 30, info, br);
	sprintf_s(info, "- bullet 5 points");
	graphics::drawText(CANVAS_WIDTH / 2 + 125, CANVAS_HEIGHT / 2 + 140, 20, info, br);
	sprintf_s(info, "- first row 10 points");
	graphics::drawText(CANVAS_WIDTH / 2 + 125, CANVAS_HEIGHT / 2 + 170, 20, info, br);
	sprintf_s(info, "- second row 20 points");
	graphics::drawText(CANVAS_WIDTH / 2 + 125, CANVAS_HEIGHT / 2 + 200, 20, info, br);
	sprintf_s(info, "- third row 30 points");
	graphics::drawText(CANVAS_WIDTH / 2 + 125, CANVAS_HEIGHT / 2 + 230, 20, info, br);
	sprintf_s(info, "- fourth row 40 points");
	graphics::drawText(CANVAS_WIDTH / 2 + 125, CANVAS_HEIGHT / 2 + 260, 20, info, br);
	sprintf_s(info, "- fifth row 50 points");
	graphics::drawText(CANVAS_WIDTH / 2 + 125, CANVAS_HEIGHT / 2 + 290, 20, info, br);
	sprintf_s(info, "- bonus enemy 200 points");
	graphics::drawText(CANVAS_WIDTH / 2 + 125, CANVAS_HEIGHT / 2 + 320, 20, info, br);
	sprintf_s(info, "- each remaining life 1000 points");
	graphics::drawText(CANVAS_WIDTH / 2 + 125, CANVAS_HEIGHT / 2 + 350, 20, info, br);

	sprintf_s(info, "1   SAFE MODE");
	graphics::drawText(340, 240, 30, info, br);
	sprintf_s(info, "2   HARD MODE");
	graphics::drawText(340, 270, 30, info, br);
	sprintf_s(info, "3   UNBEATABLE MODE");
	graphics::drawText(340, 300, 30, info, br);

	sprintf_s(info, "Press   1   2   or   3   to start");
	//sprintf_s(info, "Press SPACE to start");
	br.fill_opacity = 0.5f + 0.5 * glow;
	br.fill_secondary_opacity = 0.5f + 0.5f * glow;
	graphics::drawText(CANVAS_WIDTH / 2 - 170, CANVAS_HEIGHT / 2 + 20, 30, info, br);

	sprintf_s(info, "Controls");
	br.fill_opacity = 1.0f;
	br.fill_secondary_opacity = 1.0f;
	graphics::drawText(185, CANVAS_HEIGHT / 2 + 150, 30, info, br);

	sprintf_s(info, "- Move Left   A");
	graphics::drawText(145, CANVAS_HEIGHT / 2 + 200, 30, info, br);
	sprintf_s(info, "+ Move Right   D");
	graphics::drawText(145, CANVAS_HEIGHT / 2 + 250, 30, info, br);
	sprintf_s(info, "- Burn Them   SPACE");
	graphics::drawText(145, CANVAS_HEIGHT / 2 + 300, 30, info, br);
	sprintf_s(info, "- Pause   S");
	graphics::drawText(145, CANVAS_HEIGHT / 2 + 350, 30, info, br);
}

//Draws Game Screen.
void Game::drawLevelScreen()
{
	graphics::Brush br;
	br.texture = std::string(ASSET_PATH) + "space.png";
	br.outline_opacity = 0.0f;
	graphics::drawRect(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, CANVAS_WIDTH, CANVAS_HEIGHT, br);

	br.texture = std::string(ASSET_PATH) + "earth.png";
	graphics::setScale(0.4f, 0.5f);
	br.fill_opacity = 0.8f;
	graphics::drawRect(CANVAS_WIDTH / 7, 50, (CANVAS_WIDTH / 5) + 50, 180, br);

	graphics::setScale(1.0f, 1.0f);

	if (player)
		player->draw();

	if (bonusenemy)
		bonusenemy->draw();

	if (projectile)
		projectile->draw();
	
	if (bulletArray)
		for (int j = 0; j < 12; j++)
		{
			if (bulletArray[j])
				bulletArray[j]->draw();
		}
	
	if (enemyArray)
	{
		for (int i = 0; i < 5; i++)
		{
			for (int j = 0; j < 12; j++)
			{
				if (enemyArray[i][j] != nullptr)
					enemyArray[i][j]->draw();
			}
		}
	}

	if (life)
	{
		for (int i = 0; i < sizeof(life) / sizeof(*life); i++)
		{
			if (life[i])
				life[i]->draw();
		}

	}

	if (bar)
	{
		for (int i = 0; i < sizeof(bar) / sizeof(*bar); i++)
		{
			if (bar[i])
				bar[i]->draw();
		}
	}

	//Score and Enemies Info.
	if (player)
	{
		char info[20];
		sprintf_s(info, "SCORE   %d", score);
		graphics::drawText(20, 20, 20, info, br);

		sprintf_s(info, "cops alive   %d", alive_enemies);
		graphics::drawText(20, 40, 15, info, br);
	}
}

//Draws Pause Screen.
void Game::drawPausedScreen()
{
	graphics::Brush br;
	char info[30];
	
	br.fill_color[0] = 0.3f;
	br.fill_color[1] = 0.3f;
	br.fill_color[2] = 1.0f;
	br.fill_secondary_color[0] = 1.0;
	br.fill_secondary_color[0] = 0.2;
	br.fill_secondary_color[0] = 0.2;
	br.gradient = true;
	br.fill_opacity = 1.0f;

	sprintf_s(info, "PAUSED");
	graphics::drawText(CANVAS_WIDTH / 2 - 120, CANVAS_HEIGHT / 2, 50, info, br);
	sprintf_s(info, "PRESS S TO CONTINUE");
	graphics::drawText(CANVAS_WIDTH / 2 - 170, CANVAS_HEIGHT / 2 + 50, 30, info, br);
}

//Draws Game Over Screen, whether it was a Win or a Loss.
void Game::drawGameOverScreen()
{
	graphics::Brush br;
	float glow = sinf(graphics::getGlobalTime() / 50);
	if (alive_enemies > 0)
	{
		graphics::setOrientation(-90.0f);
		br.texture = std::string(ASSET_PATH) + "life.png";
		br.outline_opacity = 0.0f;
		graphics::drawRect(CANVAS_WIDTH / 2 - 50, CANVAS_HEIGHT / 2 + 75, 200, 200, br);
		
		graphics::setOrientation(125.0f);
		br.texture = std::string(ASSET_PATH) + "mol.png";
		br.fill_opacity = 0.9f;
		graphics::drawRect(CANVAS_WIDTH / 2 - 200, CANVAS_HEIGHT / 2 + 140, 60, 60, br);
		graphics::resetPose();

		char info[30];
		br.fill_color[0] = 1.0f;
		br.fill_color[1] = 0.0f;
		br.fill_color[2] = 0.0f;
		br.fill_secondary_color[0] = 1.0;
		br.fill_secondary_color[0] = 1.0;
		br.fill_secondary_color[0] = 1.0;
		br.gradient = true;
		sprintf_s(info, "GAME OVER");
		graphics::drawText(300, CANVAS_HEIGHT / 2 - 250, 50, info, br);
		sprintf_s(info, "THE COPS CONQUERED OUR PLANET");
		graphics::drawText(CANVAS_WIDTH / 2 - 245, CANVAS_HEIGHT / 2 - 170, 30, info, br);
		sprintf_s(info, "YOUR SCORE WAS %d POINTS", score);
		graphics::drawText(CANVAS_WIDTH / 2 - 193, CANVAS_HEIGHT / 2 - 120, 30, info, br);
		sprintf_s(info, "PRESS ESC TO EXIT");
		br.fill_opacity = 0.5f + 0.5f * glow;
		br.fill_secondary_opacity = 0.5f + 0.5f * glow;
		graphics::drawText(CANVAS_WIDTH / 2 - 193, CANVAS_HEIGHT - 70, 40, info, br);

	}
	else
	{
		br.texture = std::string(ASSET_PATH) + "life.png";
		br.outline_opacity = 0.0f;
		graphics::drawRect(CANVAS_WIDTH / 2 - 50, CANVAS_HEIGHT / 2 + 75, 200, 200, br);

		br.texture = std::string(ASSET_PATH) + "mol.png";
		br.fill_opacity = 0.7f;
		graphics::drawRect(CANVAS_WIDTH / 2 + 40 , CANVAS_HEIGHT / 2 + 90, 60, 60, br);

		char info[40];
		br.fill_color[0] = 0.3f;
		br.fill_color[1] = 0.3f;
		br.fill_color[2] = 1.0f;
		br.fill_secondary_color[0] = 1.0;
		br.fill_secondary_color[0] = 0.2;
		br.fill_secondary_color[0] = 0.2;
		br.gradient = true;
		br.fill_opacity = 1.0f;
		sprintf_s(info, "CONGRATS! YOU JUST SAVED THE EARTH!");
		graphics::drawText(20, 100, 50, info, br);
		sprintf_s(info, "YOUR SCORE WAS %d POINTS", score);
		graphics::drawText(CANVAS_WIDTH / 2 - 200, CANVAS_HEIGHT / 2 - 100, 30, info, br);
		sprintf_s(info, "PRESS ESC TO EXIT");
		br.fill_opacity = 0.5f + 0.5f * glow;
		br.fill_secondary_opacity = 0.5f + 0.5f * glow;
		graphics::drawText(CANVAS_WIDTH / 2 - 193, CANVAS_HEIGHT - 70, 40, info, br);
	}	
}

//Main Update Function
void Game::update()
{
	if (status==STATUS_START)
	{
		updateStartScreen();
	}
	else if (status == STATUS_PLAYING)
	{ 
		updateLevelScreen();
	}
	else if (status == STATUS_PAUSED)
	{
		updatePausedScreen();
	}
	else if (status == STATUS_GAMEOVER)
	{
		updateGameOverScreen();
	}
}

//Main Draw Function.
void Game::draw()
{
	if (status == STATUS_START)
	{
		drawStartScreen();
	}
	else if (status == STATUS_PLAYING)
	{
		drawLevelScreen();
	}
	else if (status == STATUS_PAUSED)
	{
		drawPausedScreen();
	}
	else if (status == STATUS_GAMEOVER)
	{
		drawGameOverScreen();
	}	
}

//Game Initialization Function. Initializes Lives, Barricades, Enemies and rows Array.
void Game::init()
{
	graphics::setFont(std::string(ASSET_PATH) + "arcadeclassic.ttf");
	graphics::playMusic(std::string(ASSET_PATH) + "cherouvim.mp3", 0.3f, true, 0);
	life[0] = new Life(*this, 20.0f);
	life[1] = new Life(*this, 60.0f);
	life[2] = new Life(*this, 100.0f);

	for (int j = 0; j < 12; j++)
	{
		rows[j] = 5;

		for (int i = 0; i < 5; i++)
		{
			bar[i] = new Barricade(*this, i);
			enemyArray[i][j] = new Enemy(*this, i, j);
		}
	}
}

Game::Game()
{

}

Game::~Game()
{
	if (player)
	{
		delete player;
		player = nullptr;
	}
}
