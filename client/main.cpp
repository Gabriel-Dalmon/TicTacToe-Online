#define WIN32_LEAN_AND_MEAN

// Include windows librairies for socket nonsense
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

// Bunch of regular, good'ol lobrairies
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <iostream>

// JSON managing librairy bcuz lazy
#include <json/json.h>
#include "../server/headers/socketRequirements.h"


// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")


#define DEFAULT_BUFLEN 132
#define DEFAULT_PORT "5150"
#define SERVER_IP "192.168.1.83"//"10.1.144.23" // 192.168.56.1 // 
#define DEFAULT_HEADSIZE    4

LPSOCKET_INFORMATION SocketInfo;

int __cdecl main(int argc, char** argv)
{

    WSADATA wsaData;
    SOCKET ConnectSocket = INVALID_SOCKET;
    struct addrinfo* result = NULL,
        * ptr = NULL,
        hints;

    // Using the JSON library to create a char* buffer.
    // This will eventually be sent back to the client, so that they can see the updated board an interact with it
    //Json::Value root;
    std::ifstream grid_snipet("game_snipet.json", std::ifstream::binary);

    //char sendbuf[DEFAULT_BUFLEN] = "";
    char sendbuf[DEFAULT_BUFLEN] = "Alpha, Beta, Charlie, Delta, Echo";
    char recvbuf[DEFAULT_BUFLEN];
    int iResult;
    int recvbuflen = DEFAULT_BUFLEN;

    // Validate the parameters
    /*if (argc != 2) {
        printf("usage: %s server-name\n", argv[0]);
        return 1;
    }*/

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
    iResult = getaddrinfo(SERVER_IP, DEFAULT_PORT, &hints, &result);
    if (iResult != 0) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // Attempt to connect to an address until one succeeds
    for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

        // Create a SOCKET for connecting to server
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
            ptr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET) {
            printf("socket failed with error: %ld\n", WSAGetLastError());
            WSACleanup();
            return 1;
        }

        // Connect to server.
        iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            printf("connect() FAILED !\n");
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    if (ConnectSocket == INVALID_SOCKET) {
        printf("Unable to connect to server!\n");
        WSACleanup();
        return 1;
    }
    else
    {
        printf("Connected to %s\n", &SERVER_IP);
    }

    // Send an initial buffer

    printf("Bytes Sent: %ld\n", iResult);


    Json::Value sendRoot;
    Json::Value request(setName);
    Json::Value reqData("HELLO, my ladies");

    sendRoot["request"] = request;
    sendRoot["reqData"] = reqData;

    Json::StreamWriterBuilder writeBuilder;
    std::string tempStr = Json::writeString(writeBuilder, sendRoot);
    const char* jsonMsg = tempStr.c_str();

    for (int i = 0; i < DEFAULT_HEADSIZE; i++)
    {
        sendbuf[i] = strlen(jsonMsg) >> (DEFAULT_HEADSIZE - 1 - i) * 8;
    }

    for (int i = DEFAULT_HEADSIZE; i < strlen(jsonMsg) + DEFAULT_HEADSIZE; i++)
    {
        sendbuf[i] = jsonMsg[i - DEFAULT_HEADSIZE];
    }

    iResult = send(ConnectSocket, sendbuf, (int)strlen(jsonMsg) + DEFAULT_HEADSIZE, 0);
    if (iResult == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }

    printf("Bytes Sent: %ld\n", iResult);

    // shutdown the connection since no more data will be sent
    /*iResult = shutdown(ConnectSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }*/

    // Receive until the peer closes the connection
    do {

        iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
        if (iResult > 0)
            printf("Bytes received: %d\n", iResult);
        else if (iResult == 0)
            printf("Connection closed\n");
        else
            printf("recv failed with error: %d\n", WSAGetLastError());

    } while (iResult > 0);

    // cleanup
    closesocket(ConnectSocket);
    WSACleanup();

    return 0;
}