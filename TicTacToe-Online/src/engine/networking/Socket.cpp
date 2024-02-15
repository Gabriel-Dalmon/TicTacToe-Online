#include "Socket.h"

#include <iostream>

Socket::Socket(HWND hwnd)
{
    ZeroMemory(&m_addressInfo, sizeof(m_addressInfo));
    m_HWND = hwnd;
    //WSASocket(PF_INET, SOCK_STREAM, IPPROTO_TCP, nullptr, 0, WSA_FLAG_OVERLAPPED)) //socket(PF_INET, SOCK_STREAM, 0)
    if ((m_winSocket = WSASocket(PF_INET, SOCK_STREAM, IPPROTO_TCP, nullptr, 0, WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET)
    {
        std::cout << "socket() failed with error " << WSAGetLastError() << "\n";
        return;
    }
    std::cout << "socket() is pretty fine!\n";

    if (WSAAsyncSelect(m_winSocket, m_HWND, WM_SOCKET, FD_CONNECT | FD_READ | FD_WRITE | FD_CLOSE) == 0)
        std::cout << "WSAAsyncSelect() is OK lol!\n";
    else
        std::cout << "WSAAsyncSelect() failed with error code " << WSAGetLastError() << "\n";
}

void Socket::Connect(const char* cServerIp, int iPort)
{
    m_addressInfo.sin_family = AF_INET;
    m_addressInfo.sin_addr.s_addr = inet_addr(cServerIp);
    m_addressInfo.sin_port = htons(iPort);

    WSAConnect(m_winSocket, (PSOCKADDR)&m_addressInfo, sizeof(m_addressInfo), NULL, NULL, NULL, NULL);
}

void Socket::Close()
{
    //GlobalFree(m_winSocketInfo);
    closesocket(m_winSocket);
}

int Socket::OnConnect()
{
    std::cout << "Connected to server" << std::endl;
    return 0;
}

int Socket::OnDisconnect()
{
    std::cout << "Disconnected from server" << std::endl;
    Close();
    return 0;
}

int Socket::OnReceive()
{

    DWORD Flags = 0;

    LPWSABUF pRecvHeaderBuf = new WSABUF;
    pRecvHeaderBuf->buf = (char*)calloc(HEADER_SIZE, sizeof(char));
    pRecvHeaderBuf->len = HEADER_SIZE;

    LPWSAOVERLAPPED pCallbackData = GetWSAOverlappedStruct(&Socket::OnHeaderReceived, (void*)pRecvHeaderBuf);

    if (WSARecv(m_winSocket, pRecvHeaderBuf, 1, NULL, &Flags, pCallbackData, &Socket::WSAAsyncCallbacksHandler) == SOCKET_ERROR)
    {
        if (WSAGetLastError() != WSAEWOULDBLOCK)
        {
            std::cout << "WSARecv() failed with error " << WSAGetLastError() << "\n";
            Close();
            return 0;
        }
        return 0;
    }
    if (HasOverlappedIoCompleted(pCallbackData)) {
        Socket::WSAAsyncCallbacksHandler(0, 0, pCallbackData, 0);
        std::cout << "Was Synchronous\n";
    }
    return 0;
}

void Socket::OnHeaderReceived(void* rawHeaderBuffer)
{
    DWORD Flags = 0;

    LPWSABUF pHeaderBuffer = (LPWSABUF)rawHeaderBuffer;
    int dataLength = 0;
    for (int i = 0; i < HEADER_SIZE; i++)
    {
        dataLength |= pHeaderBuffer->buf[i] << (HEADER_SIZE - 1 - i) * 8;
    }

    LPWSABUF pRecvDataBuf = new WSABUF;
    pRecvDataBuf->buf = (char*)malloc(sizeof(char) * dataLength);
    pRecvDataBuf->len = dataLength;

    LPWSAOVERLAPPED pCallbackData = GetWSAOverlappedStruct(&Socket::OnDataReceived, (void*)pRecvDataBuf);

    if (WSARecv(m_winSocket, pRecvDataBuf, 1, NULL, &Flags, pCallbackData, &Socket::WSAAsyncCallbacksHandler) == SOCKET_ERROR)
    {
        if (WSAGetLastError() != WSAEWOULDBLOCK)
        {
            std::cout << "WSARecv() failed with error" << WSAGetLastError() << "\n";
            //Close();
            return;
        }
    }
    else // No error so update the byte count
    {
        std::cout << "WSARecv() is OK!\n";
    }
    if (HasOverlappedIoCompleted(pCallbackData)) {
        Socket::WSAAsyncCallbacksHandler(0, 0, pCallbackData, 0);
        std::cout << "Was Synchronous\n";
    }
}

void Socket::OnDataReceived(void* rawDataBuffer)
{
    LPWSABUF pRecvDataBuf = (LPWSABUF)rawDataBuffer;
    // Should be moved to namespace utils {
    Json::Value jsonData;
    JSONCPP_STRING err;
    Json::CharReaderBuilder readBuilder;
    const std::unique_ptr<Json::CharReader> reader(readBuilder.newCharReader());
    if (!reader->parse(pRecvDataBuf->buf, pRecvDataBuf->buf + strlen(pRecvDataBuf->buf), &jsonData, &err))
    {
        std::cout << "Error while parsing received data to JSON." << std::endl;
        return;
    }
    // }

    /*switch (jsonData["request"].asInt())
    {
        case setName:
            printf("\n\tWe should set the name with %s\n", jsonData["reqData"].asCString());
            break;
        case makePlay:
            printf("\n\tWe should set the name with %s\n", jsonData["reqData"].asInt());
            break;
    }*/
    std::cout << "Data successfully received and handled\n" << jsonData << "\n";
}

int Socket::OnSend()
{
    std::cout << "OnSend() called\n";
    if (m_sendQueue.empty()) return 0;
    LPWSABUF pFullSendBuffer = m_sendQueue.front();
    m_sendQueue.pop();

    LPWSAOVERLAPPED pCallbackData = GetWSAOverlappedStruct(&Socket::DeleteBuffer, (void*)pFullSendBuffer);

    int iResult = WSASend(m_winSocket, pFullSendBuffer, 1, NULL, WSA_FLAG_OVERLAPPED,
        pCallbackData, &Socket::WSAAsyncCallbacksHandler);
    if (iResult == SOCKET_ERROR)
    {
        if (WSAGetLastError() != WSAEWOULDBLOCK)
        {
            std::cout << "WSASend() failed with error " << WSAGetLastError() << "\n";
            Close();
            return 0;
        }
        return 0;
    }
    else // No error so update the byte count
    {
        std::cout << "WSASend() is OK!\n";
    }
    if (HasOverlappedIoCompleted(pCallbackData)) {
        Socket::WSAAsyncCallbacksHandler(0, 0, pCallbackData, 0);
        std::cout << "Was Synchronous\n";
    }
    return 0;
}

LPWSAOVERLAPPED Socket::GetWSAOverlappedStruct(void (Socket::* methodPointer)(HANDLE arguments), void* arguments)
{
    LPWSAOVERLAPPED pCallbackData = new OVERLAPPED;
    CallbackData* WSASendCallback = new CallbackData;
    WSASendCallback->instance = this;
    WSASendCallback->methodPointer = methodPointer;
    WSASendCallback->arguments = arguments;
    pCallbackData->hEvent = WSASendCallback;
    return pCallbackData;
}

void Socket::SendJSON(int requestType, Json::Value JSONRequestData)
{
    Json::Value JSONSendData;
    JSONSendData["requestType"] = requestType;
    JSONSendData["reqData"] = JSONRequestData;
    Json::StreamWriterBuilder writeBuilder;
    std::string tempStr = Json::writeString(writeBuilder, JSONSendData);
    const char* cData = tempStr.c_str();
    int iDataLength = (int)strlen(cData);

    /*
    * char cHeader[HEADER_SIZE];
    if(sizeof(iDataLength)) return;
    */
    //Possible variable bytes order
    //memcpy(cHeader, &x, sizeof(iDataLength));
    //eventually : adjustEndianess(cHeader, sizeof(cHeader));
    //memset(cHeader + sizeof(iDataLength), 0, sizeof(cHeader) - sizeof(iDataLength));

    //Freed in WSASend OVERLAPPED COROUTINE
    char* sendBuffer = (char*)malloc((HEADER_SIZE + iDataLength) * sizeof(char));
    if (sendBuffer == NULL) {
        return;
    }

    // Fills sendbuf with the size of the message to send as an int, into the char* so that send() can work with it.
    for (int i = 0; i < HEADER_SIZE; i++)
    {
        sendBuffer[i] = iDataLength >> (HEADER_SIZE - 1 - i) * 8;
    }
    // Fills manually sendbuf with the rest of the message
    // C/C++ default concatenators fail here because we are often dealing with '\0', hence the manual option
    for (int i = HEADER_SIZE; i < iDataLength + HEADER_SIZE; i++)
    {
        sendBuffer[i] = cData[i - HEADER_SIZE];
    }

    LPWSABUF pFullSendBuffer = new WSABUF;
    pFullSendBuffer->buf = sendBuffer;
    pFullSendBuffer->len = HEADER_SIZE + iDataLength;
    m_sendQueue.push(pFullSendBuffer);
    PostMessage(m_HWND, WM_SOCKET, m_winSocket, FD_WRITE);
    std::cout << "Data sent\n";
}

void CALLBACK Socket::WSAAsyncCallbacksHandler(DWORD dwError, DWORD cbTransferred, LPWSAOVERLAPPED lpOverlapped, DWORD dwFlags)
{
    std::cout << "Callback called\n";
    CallbackData* pCallbackData = (CallbackData*)(lpOverlapped->hEvent);
    ((pCallbackData->instance)->*(pCallbackData->methodPointer))(pCallbackData->arguments);
    delete pCallbackData;
    delete lpOverlapped;
}

void Socket::DeleteBuffer(void* rawFullBuffer)
{
    LPWSABUF pFullBuffer = (LPWSABUF)rawFullBuffer;
    free(pFullBuffer->buf);
    delete pFullBuffer;
}

Socket::~Socket()
{

}

