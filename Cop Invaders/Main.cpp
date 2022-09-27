#include "graphics.h"
#include "Game.h"
#include "config.h"

void update(float ms)
{
    Game* game = reinterpret_cast<Game *> (graphics::getUserData());
    game->update();
}


void draw()
{
    Game* game = reinterpret_cast<Game*> (graphics::getUserData());
    game->draw();
}

int main()
{
    Game mygame;
    
    graphics::createWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Cop Invaders");

    graphics::setUserData(&mygame);

    graphics::setDrawFunction(draw);
    graphics::setUpdateFunction(update);

    graphics::setCanvasSize(CANVAS_WIDTH, CANVAS_HEIGHT);
    graphics::setCanvasScaleMode(graphics::CANVAS_SCALE_FIT);
    graphics::setFullScreen(true);
    
    mygame.init();

    graphics::startMessageLoop();

    graphics::destroyWindow();


    return 0;
}