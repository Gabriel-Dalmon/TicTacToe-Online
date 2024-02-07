#include "engine/core/GameManager.h"
#include "engine/rendering/Window.h"
#include "engine/textures/TextureManager.h"
#include <iostream>
#include "./gameStates/PlayGameState.h"

int main(int argc, char** argv)
{
    
    std::srand(static_cast<unsigned int>(std::time(0)));
    Window* pWindow = new Window(900, 700, "TicTacToe");
    GameManager::instance().InitGameManager(pWindow, new PlayGameState(pWindow));

    //GameLoop
    while (GameManager::instance().getWindow()->getSFMLObject()->isOpen())
    {

        //UPDATE
        GameManager::instance().update();

        //renvoie
        //GameManager::instance().render();

    }

    TextureManager::destroyTexture();

    return 0;
}