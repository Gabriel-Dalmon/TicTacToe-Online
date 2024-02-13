#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <conio.h>
#include <utility>
#include <iostream>

#include "socketRequirements.h"
#include "Thread.h"



//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
int main(int argc, char** argv)
{

    
    WSADATA wsaData;
    Thread test(&wsaData);

    while (true) {
        //std::cout << Thread::threadList.size() << std::endl;
    }

}