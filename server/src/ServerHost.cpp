#include "ServerHost.hpp"



// Need to link with Ws2_32.lib

// #pragma comment (lib, "Mswsock.lib")



ServerHost::ServerHost() {

    ListenSocket = INVALID_SOCKET;

    addrinfo* result = NULL;
    addrinfo hints;

    recvbuflen = DEFAULT_BUFLEN;

    socketSetup();
};

ServerHost::~ServerHost() {
    closesocket(ListenSocket);

    // shutdown the connections since we're done
    for (auto client : clientList)
    {
        iResult = shutdown(client, SD_SEND);
        if (iResult == SOCKET_ERROR) {
            printf("shutdown failed with error: %d\n", WSAGetLastError());
            closesocket(client);
        }
    }
    WSACleanup();
};

int ServerHost::socketSetup() {
    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the server address and port
    iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
    if (iResult != 0) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // Create a SOCKET for the server to listen for client connections.
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }

    // Setup the TCP listening socket
    iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    freeaddrinfo(result);

    iResult = listen(ListenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    return 0;
}


// Accept a new client
int __cdecl ServerHost::host()
{
    while (clientList.size() < 1)
    {
        SOCKET tempClientSocket = accept(ListenSocket, NULL, NULL);
        if (tempClientSocket == INVALID_SOCKET) {
            printf("accept failed with error: %d\n", WSAGetLastError());
            closesocket(ListenSocket);
            WSACleanup();
            return 1;
        }
        else {
            clientList.push_back(tempClientSocket);
        }
    }
    hosting = true;

    return 0;
}

void ServerHost::recieveFrom(SOCKET* client) {

    // Getting the size of the message we need to read as char* (which are as good as unsigned char* at representing Bytes)
    char headerSize[DEFAULT_HEADSIZE];
    iResult = recv(*client, headerSize, DEFAULT_HEADSIZE, 0);   // Stores every Byte determining the message's size in headerSize
    if (iResult > 0)
    {
        printf("Bytes read: %d\n", iResult);
        std::cout << "Here is headerSize as char : " << (char)headerSize << "\n";
    }
    else if (iResult == 0)
    {
        printf("Message empty\n");
    }
    else
    {
        printf("recv failed with error: %d\n", WSAGetLastError());
        //closesocket(*client);
    }
    
    // Transforming the char* headerSize "Bytes" into an integer usable again by recv()
    int dataBufferSize = 0;
    for (int i = 0;  i < DEFAULT_HEADSIZE;  i++)
    {
        printf("   /!\\ Entering for() loop ! Iteration number %d", i + 1);
        dataBufferSize |= headerSize[i] << i * 8;
        printf("      dataBufferSize is now %08X\n", dataBufferSize);
    }

    printf("\nfor() loop exited ! dataBufferSize is now 0x%08X, or %d", dataBufferSize, dataBufferSize);

    //iResult = recv(*client, recvbuf, dataBufferSize, 0);
    //if (iResult > 0) {
    //    printf("Bytes received: %d\n", iResult);
    //    std::cout << recvbuf << std::endl;
    //}
    //else if (iResult == 0) {
    //    printf("Message empty\n");
    //    Sleep(1000);
    //}
    //else {
    //    printf("recv failed with error: %d\n", WSAGetLastError());
    //    //closesocket(*client);
    //}
}

void ServerHost::sendTo(SOCKET* client) {
    iSendResult = send(*client, recvbuf, iResult, 0);
    if (iSendResult == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        //closesocket(clientList[i]);
    }
    printf("Bytes sent: %d\n", iSendResult);
}