#include "engine/core/GameManager.h"
#include "engine/rendering/Window.h"
#include "engine/textures/TextureManager.h"
#include <iostream>
#include <windows.h>
#include "./gameStates/PlayGameState.h"

int main(int argc, char** argv)
{
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData)) {
        std::cout << "WSAStartup failed with error: %d\n" << WSAGetLastError() << "\n";
        return 1;
    }

    std::srand(static_cast<unsigned int>(std::time(0)));
    Window* pWindow = new Window(900, 700, "TicTacToe");
    GameManager::instance().InitGameManager(pWindow, new PlayGameState(pWindow));

    //GameLoop
    while (GameManager::instance().getWindow()->getSFMLObject()->isOpen())
    {
        //UPDATE
        GameManager::instance().update();

        //DISPLAY
        GameManager::instance().render();

    }

    WSACleanup();
    TextureManager::destroyTexture();

    return 0;
}