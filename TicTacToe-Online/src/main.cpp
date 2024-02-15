#include "engine/core/GameManager.h"
#include "engine/rendering/Window.h"
#include "engine/textures/TextureManager.h"
#include <iostream>
#include "./gameStates/PlayGameState.h"
#include "./engine/networking/SocketThread.h"

#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define NOMINMAX


#include <winsock2.h>
#include <windows.h>

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

int main(int argc, char** argv)
{
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData)) {
        std::cout << "WSAStartup failed with error: %d\n" << WSAGetLastError() << "\n";
        WSACleanup();
        return 1;
    }
    SocketThread* pSocketThread = new SocketThread();
    pSocketThread->Run();

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

    TextureManager::destroyTexture();

    WSACleanup();
    return 0;
}