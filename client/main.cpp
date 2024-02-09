#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS

// Include windows librairies for socket nonsense
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

// Bunch of regular, good'ol lobrairies
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <conio.h>

// JSON managing librairy
#include <json/json.h>


// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")


#define DEFAULT_BUFLEN 1
#define DEFAULT_PORT "6942"
#define SERVER_IP "192.168.212.22"

#define DEFAULT_BUFLEN 512
#define DEFAULT_HEADSIZE 4

enum RequestType
{
    setName = 0,
    makePlay
};

int __cdecl main(int argc, char** argv)
{
    WSADATA wsaData;
    SOCKET ConnectSocket = INVALID_SOCKET;
    struct addrinfo* result = NULL,
        * ptr = NULL,
        hints;

    /*
     *  Hopefully Json to char* transformation :P
     */
    // Sets the value that are gonna end up in the Json
    Json::Value root;
    Json::Value request(setName);
    Json::Value reqData("Jose");

    // Fills up the Json
    root["request"] = request;
    root["reqData"] = reqData;

    // Converts the Json from std::string to const char*
    Json::StreamWriterBuilder builder;
    std::string tempStr = Json::writeString(builder, root);
    const char* jsonMsg = tempStr.c_str();
    printf("jsonMsg : %s\n", jsonMsg);





    /*
     *  This big block of code take a char* variable, and uses it's size as well as 2 constants to pack it into 1 char* usuable by msdn::send()
     *  The 2 constants used are to define the max size of the message that will be sent, as well as the fix size of the header to read the data
     *  At 4, the header is the size of one(1) int. The size of the message is expressed in Bytes.
     * 
     *  Once the Json format will be in place, this block will work assuming we're dealing with the constant amount of Byte defined above on both client and server side
     */
    char sendbuf[DEFAULT_BUFLEN] = "";
    // Fills sendbuf with the size of the message to send as an int, into the char* so that send() can work with it.
    printf("sendbuf : \n");
    for (int i = 0; i < DEFAULT_HEADSIZE; i++)
    {
        sendbuf[i] = strlen(jsonMsg) >> (DEFAULT_HEADSIZE - 1 - i) * 8;
        printf("\t[%d] = %02X, or %c\n", i, sendbuf[i], sendbuf[i]);
    }
    // Fills manually sendbuf with the rest of the message
    // C/C++ default concatenators fail here because we are often dealing with '\0', hence the manual option
    for (int i = DEFAULT_HEADSIZE;  i < strlen(jsonMsg) + DEFAULT_HEADSIZE;  i++)
    {
        sendbuf[i] = jsonMsg[i - DEFAULT_HEADSIZE];
        printf("\t[%d] = %02X, or %c\n", i, sendbuf[i], sendbuf[i]);
    }







    //const char* sendbuf = json_file.c_str();
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

    //// Asks for where the player want to play
    //printf("Wanna break from the ads ? ");
    //char input;
    //input = _getch();
    //sendbuf = &input;

    // Send an initial buffer
    iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
    if (iResult == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }

    printf("Bytes Sent: %ld\n", iResult);

    // shutdown the connection since no more data will be sent
    iResult = shutdown(ConnectSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }

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