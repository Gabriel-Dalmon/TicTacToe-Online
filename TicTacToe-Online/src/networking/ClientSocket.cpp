#include "ClientSocket.h"

#define WIN32_LEAN_AND_MEAN

#include "../engine/core/GameManager.h"
#include "../engine/events/EventsManager.h"


#include <json/json.h>
#include <iostream>

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")


#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "6942"
#define SERVER_IP "10.1.144.23"
#define HEADER_LENGTH 0//5
#define DEFAULT_BUFFER_LENGTH 127

ClientSocket::ClientSocket()
{
    
    struct addrinfo hints;

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    int iResult = getaddrinfo(SERVER_IP, DEFAULT_PORT, &hints, &m_ServerAdressInfo);
    if (iResult != 0) {
        std::cout << "getaddrinfo failed with error : % d\n" << iResult << "\n";
    }
}

ClientSocket::~ClientSocket()
{
    closesocket(m_ConnectSocket);
}

int ClientSocket::connectSocket() {
    return connect(m_ConnectSocket, m_ServerAdressInfo->ai_addr, (int)m_ServerAdressInfo->ai_addrlen);
}

void ClientSocket::generateBuffer(Json::Value json) {
    Json::StreamWriterBuilder builder;
    const std::string json_file = Json::writeString(builder, json);
    std::cout << json_file << "\n" << json_file.length() << std::endl;
    unsigned char b = (unsigned char)json_file.length();
    std::cout << "\n" << b << std::endl;
}

Json::Value ClientSocket::recvJSON() {
    int iResult;
    char headerBuffer[HEADER_LENGTH + 1];
    char contentBuffer;
    do { 
        //iResult = recv(m_ConnectSocket, headerBuffer, HEADER_LENGTH, 0);
        recv(m_ConnectSocket, &contentBuffer, DEFAULT_BUFFER_LENGTH, 0);
        //GameManager::instance().getEventsManager()->trigger(UPDATE_GRID);
        if (iResult > 0)
            std::cout << "Header received: %d\n" << iResult;
        else if (iResult == 0)
            printf("Connection closed\n");
        else
            printf("recv failed with error: %d\n", WSAGetLastError());

    } while (iResult > 0);
}

const char* ClientSocket::serializeToBytes(Json::Value jsonValue) {
    Json::StreamWriterBuilder builder;
    const std::string json_file = Json::writeString(builder, jsonValue);
    return json_file.c_str();
}

Json::Value ClientSocket::deserializeToJSON(char* pBytesValue) {
    std::string rawJson = pBytesValue;
    const int rawJsonLength = static_cast<int>(rawJson.length());
    Json::Value deserializedJSON;
    JSONCPP_STRING err;

    Json::CharReaderBuilder builder;
    const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
    
    if (!reader->parse(rawJson.c_str(), rawJson.c_str() + rawJsonLength, &deserializedJSON,
        &err)) {
        return "-1";
    }
}

void ClientSocket::sendJSON(Json::Value jsonValue) {
    const char* sendBuffer = this->serializeToBytes(jsonValue);
    int bufferContentLength = (int)strlen(sendBuffer);
    //add header    
    send(m_ConnectSocket, sendBuffer, HEADER_LENGTH + bufferContentLength, 0);
};
