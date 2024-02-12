#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <winsock2.h>
#include <Windows.h>
#include <stdio.h>
#include <utility>
#include <iostream>
#include <json/json.h>

#include "socketRequirements.hpp"



LPSOCKET_INFORMATION SocketInfoList;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)

{
    SOCKET Accept;
    LPSOCKET_INFORMATION SocketInfo;
    DWORD RecvBytes;
    DWORD SendBytes;
    DWORD Flags;
    if (uMsg == WM_SOCKET)
    {
        if (WSAGETSELECTERROR(lParam))
        {
            printf("Socket failed with error %d\n", WSAGETSELECTERROR(lParam));
            FreeSocketInformation(wParam);
        }
        else
        {
            printf("Socket looks fine!\n");
            switch (WSAGETSELECTEVENT(lParam))
            {
            case FD_ACCEPT:
                //printf("FD_ACCEPT Called");
                if ((Accept = accept(wParam, NULL, NULL)) == INVALID_SOCKET)
                {
                    printf("accept() failed with error %d\n", WSAGetLastError());
                    break;
                }
                else
                    printf("accept() is OK!\n");
                // Create a socket information structure to associate with the socket for processing I/O
                CreateSocketInformation(Accept);
                printf("Socket number %d connected\n", Accept);
                WSAAsyncSelect(Accept, hwnd, WM_SOCKET, FD_READ | FD_WRITE | FD_CLOSE);
                break;

            case FD_READ:
                printf("FD_READ is OK!\n");
                SocketInfo = GetSocketInformation(wParam);
                // Read data only if the receive buffer is empty
                if (SocketInfo == NULL) {
                    break;
                }
                if (SocketInfo->BytesRECV != 0)
                {
                    SocketInfo->RecvPosted = TRUE;
                    return 0;
                }
                else
                {
                    SocketInfo->DataBuf.buf = (CHAR*)"1234";
                    SocketInfo->DataBuf.len = DEFAULT_HEADERSIZE;
                    Flags = 0;
                    if (WSARecv(SocketInfo->Socket, &(SocketInfo->DataBuf), 1, &RecvBytes,
                        &Flags, NULL, NULL) == SOCKET_ERROR)
                    {
                        if (WSAGetLastError() != WSAEWOULDBLOCK)
                        {
                            printf("WSARecv() failed with error %d\n", WSAGetLastError());
                            FreeSocketInformation(wParam);
                            return 0;
                        }
                    }
                    else // No error so update the byte count
                    {
                        WSABUF SockInfo;
                        int headerSize = 0;
                        for (int i = 0; i < DEFAULT_HEADERSIZE; i++)
                        {
                            printf("char header[%d] : %02X\n", i, SocketInfo->DataBuf.buf[i]);
                            headerSize |= SocketInfo->DataBuf.buf[i] << (DEFAULT_HEADERSIZE - 1 - i) * 8;
                            printf("int headerSize : %08X\n\n", headerSize);
                        }
                        char* recievedData = (char*)malloc(headerSize * sizeof(char));
                        SockInfo.buf = recievedData;
                        SockInfo.len = headerSize;
                        if (WSARecv(SocketInfo->Socket, &(SockInfo), 1, &RecvBytes,
                            &Flags, NULL, NULL) == SOCKET_ERROR)
                        {
                            if (WSAGetLastError() != WSAEWOULDBLOCK)
                            {
                                free(recievedData);
                                printf("WSARecv() failed with error %d\n", WSAGetLastError());
                                FreeSocketInformation(wParam);
                                return 0;
                            }
                        }

                        /*
                          *  Convertion in the other direction because server is f* cked
                          */

                          // Convertion into std::string so that the Json library can use it
                        std::string json_data(recievedData);

                        // Parsing the Json data
                        Json::Value recieveRoot;
                        JSONCPP_STRING err;
                        Json::CharReaderBuilder readBuilder;
                        const std::unique_ptr<Json::CharReader> reader(readBuilder.newCharReader());
                        if (!reader->parse(json_data.c_str(), json_data.c_str() + json_data.length(), &recieveRoot, &err))
                        {
                            std::cout << "error" << std::endl;
                        }

                        // Extracting the data
                        switch (recieveRoot["request"].asInt())
                        {
                        case setName:
                            printf("\n\tWe should set the name with %s\n", recieveRoot["reqData"].asCString());
                            break;
                        case makePlay:
                            printf("\n\tWe should set the name with %s\n", recieveRoot["reqData"].asInt());
                            break;
                        }


                        std::cout << "WSARecv() is OK!\n" << recievedData << "\n";
                        SocketInfo->BytesRECV = RecvBytes;
                        free(recievedData);
                    }
                }
                // DO NOT BREAK HERE SINCE WE GOT A SUCCESSFUL RECV. Go ahead
                // and begin writing data to the client
            case FD_WRITE:
                printf("FD_WRITE is OK!\n");
                SocketInfo = GetSocketInformation(wParam);
                if (SocketInfo->BytesRECV > SocketInfo->BytesSEND)
                {

                    Json::Value sendRoot;
                    Json::Value request(setName);
                    Json::Value reqData("Jose");

                    // Fills up the Json
                    sendRoot["request"] = request;
                    sendRoot["reqData"] = reqData;

                    // Converts the Json from std::string to const char*
                    Json::StreamWriterBuilder writeBuilder;
                    std::string tempStr = Json::writeString(writeBuilder, sendRoot);
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
                    for (int i = 0; i < DEFAULT_HEADERSIZE; i++)
                    {
                        sendbuf[i] = strlen(jsonMsg) >> (DEFAULT_HEADERSIZE - 1 - i) * 8;
                        printf("%02X", sendbuf[i]);
                    }
                    // Fills manually sendbuf with the rest of the message
                    // C/C++ default concatenators fail here because we are often dealing with '\0', hence the manual option
                    for (int i = DEFAULT_HEADERSIZE; i < strlen(jsonMsg) + DEFAULT_HEADERSIZE; i++)
                    {
                        sendbuf[i] = jsonMsg[i - DEFAULT_HEADERSIZE];
                        printf("%c", sendbuf[i]);
                    }

                    SocketInfo->DataBuf.buf = sendbuf;
                    SocketInfo->DataBuf.len = strlen(sendbuf);

                    //strncpy_s(SI->Buffer, "Hello", strlen("Hello"));

                    if (WSASend(SocketInfo->Socket, &(SocketInfo->DataBuf), 1, &SendBytes, 0,
                        NULL, NULL) == SOCKET_ERROR)
                    {
                        if (WSAGetLastError() != WSAEWOULDBLOCK)
                        {
                            printf("WSASend() failed with error %d\n", WSAGetLastError());
                            FreeSocketInformation(wParam);
                            return 0;
                        }
                    }
                    else // No error so update the byte count
                    {
                        printf("WSASend() is OK!\n");
                        SocketInfo->BytesSEND += SendBytes;
                    }
                }
                if (SocketInfo->BytesSEND == SocketInfo->BytesRECV)
                {
                    SocketInfo->BytesSEND = 0;
                    SocketInfo->BytesRECV = 0;
                    // If a RECV occurred during our SENDs then we need to post an FD_READ notification on the socket
                    if (SocketInfo->RecvPosted == TRUE)
                    {
                        SocketInfo->RecvPosted = FALSE;

                        PostMessage(hwnd, WM_SOCKET, wParam, FD_READ);
                    }
                }
                //PostMessage(hwnd, WM_SOCKET, wParam, FD_WRITE);
                break;



            case FD_CLOSE:

                printf("Closing socket %d\n", wParam);

                FreeSocketInformation(wParam);
                break;
            }
        }
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void CreateSocketInformation(SOCKET s)
{
    LPSOCKET_INFORMATION SI;
    if ((SI = (LPSOCKET_INFORMATION)GlobalAlloc(GPTR, sizeof(SOCKET_INFORMATION))) == NULL)
    {
        printf("GlobalAlloc() failed with error %d\n", GetLastError());
        return;
    }
    else printf("GlobalAlloc() for SOCKET_INFORMATION is OK!\n");

    // Prepare SocketInfo structure for use
    SI->Socket = s;
    SI->RecvPosted = FALSE;
    SI->BytesSEND = 0;
    SI->BytesRECV = 0;
    SI->Next = SocketInfoList;
    SocketInfoList = SI;
}

LPSOCKET_INFORMATION GetSocketInformation(SOCKET s)
{
    SOCKET_INFORMATION* SI = SocketInfoList;
    while (SI)
    {
        if (SI->Socket == s)
            return SI;
        SI = SI->Next;
    }
    return NULL;
}

void FreeSocketInformation(SOCKET s)
{
    SOCKET_INFORMATION* SI = SocketInfoList;
    SOCKET_INFORMATION* PrevSI = NULL;
    while (SI)
    {
        if (SI->Socket == s)
        {
            if (PrevSI)
                PrevSI->Next = SI->Next;
            else
                SocketInfoList = SI->Next;
            closesocket(SI->Socket);
            GlobalFree(SI);
            return;
        }
        PrevSI = SI;
        SI = SI->Next;
    }
}

HWND MakeWorkerWindow(void)
{
    WNDCLASS wndclass;
    const CHAR* ProviderClass = "AsyncSelect";
    HWND Window;
    wndclass.style = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc = (WNDPROC)WindowProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = NULL;
    wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = (LPCWSTR)ProviderClass;

    if (RegisterClass(&wndclass) == 0)
    {
        printf("RegisterClass() failed with error %d\n", GetLastError());
        return NULL;
    }
    else printf("RegisterClass() is OK!\n");

    // Create a window
    //CreateWindow()
    if ((Window = CreateWindow(

        (LPCWSTR)ProviderClass,

        L"TestWindow",

        WS_OVERLAPPEDWINDOW,

        CW_USEDEFAULT,

        CW_USEDEFAULT,

        CW_USEDEFAULT,

        CW_USEDEFAULT,

        NULL,

        NULL,

        NULL,

        NULL)) == NULL)
    {
        printf("CreateWindow() failed with error %d\n", GetLastError());
        return NULL;
    }
    else printf("CreateWindow() is OK!\n");
    return Window;
}


std::pair<SOCKET, HWND> WindowSocketInitialize(WSADATA* wsaData) {
    HWND Window;
    MSG msg;
    DWORD Ret;
    SOCKET Listen;
    SOCKADDR_IN InternetAddr;


    if ((Window = MakeWorkerWindow()) == NULL)

    {

        printf("MakeWorkerWindow() failed!\n");

        return std::pair<SOCKET, HWND>(INVALID_SOCKET, NULL);

    }
    else printf("MakeWorkerWindow() is OK!\n");
    // Prepare echo server

    if (WSAStartup((2, 2), wsaData) != 0)

    {

        printf("WSAStartup() failed with error %d\n", WSAGetLastError());

        return std::pair<SOCKET, HWND>(INVALID_SOCKET, NULL);

    }

    else

        printf("WSAStartup() is OK!\n");



    if ((Listen = socket(PF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)

    {

        printf("socket() failed with error %d\n", WSAGetLastError());

        return std::pair<SOCKET, HWND>(INVALID_SOCKET, NULL);

    }

    else

        printf("socket() is pretty fine!\n");



    if (WSAAsyncSelect(Listen, Window, WM_SOCKET, FD_ACCEPT | FD_CLOSE) == 0)

        printf("WSAAsyncSelect() is OK lol!\n");

    else

        printf("WSAAsyncSelect() failed with error code %d\n", WSAGetLastError());



    InternetAddr.sin_family = AF_INET;

    InternetAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    InternetAddr.sin_port = htons(PORT);



    if (bind(Listen, (PSOCKADDR)&InternetAddr, sizeof(InternetAddr)) == SOCKET_ERROR)

    {

        printf("bind() failed with error %d\n", WSAGetLastError());

        return std::pair<SOCKET, HWND>(INVALID_SOCKET, NULL);

    }
    else printf("bind() is OK maaa!\n");

    if (listen(Listen, 5))
    {
        printf("listen() failed with error %d\n", WSAGetLastError());
        return std::pair<SOCKET, HWND>(INVALID_SOCKET, NULL);
    }
    else
        printf("listen() is also OK! I am listening now...\n");
    // Translate and dispatch window messages for the application thread

    while (Ret = GetMessage(&msg, NULL, 0, 0))

    {
        if (Ret == -1)
        {
            printf("\nGetMessage() failed with error %d\n", GetLastError());
            //return 1;
        }
        else printf("\nGetMessage() is pretty fine!\n");

        printf("Translating a message...\n");
        TranslateMessage(&msg);
        printf("Dispatching a message...\n");
        DispatchMessage(&msg);
    }

    return std::pair<SOCKET, HWND>(Listen, Window);
}