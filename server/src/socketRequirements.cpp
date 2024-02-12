#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <winsock2.h>
#include <Windows.h>
#include <stdio.h>
#include <utility>
#include <iostream>
#include <json/json.h>
#include <process.h>

#include "socketRequirements.hpp"


HANDLE  handleThreads[MAX_THREADS] = { NULL };
int threadNumber = 0;
LPSOCKET_INFORMATION SocketInfoList;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)

{
    LPREQUEST_DATA requestArgs = CreateNewRequestArgs(wParam);
    
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
                if ((*requestArgs->Accept = accept(wParam, NULL, NULL)) == INVALID_SOCKET)
                {
                    printf("accept() failed with error %d\n", WSAGetLastError());
                    break;
                }
                else
                    printf("accept() is OK!\n");
                // Create a socket information structure to associate with the socket for processing I/O
                CreateSocketInformation(*requestArgs->Accept);
                printf("Socket number %d connected\n", *requestArgs->Accept);
                WSAAsyncSelect(*requestArgs->Accept, hwnd, WM_SOCKET, FD_READ | FD_WRITE | FD_CLOSE);
                break;

            case FD_READ:
                threadNumber++;
                handleThreads[threadNumber] = (HANDLE)_beginthread((_beginthread_proc_type)ReadRequest, 0, (void*)requestArgs);
                
                // readRequest(LPSOCKET_INFORMATION* SocketInfo, WPARAM* wParam, DWORD* RecvBytes, DWORD* Flags)
            case FD_WRITE:
                threadNumber++;
                handleThreads[threadNumber] = (HANDLE)_beginthread((_beginthread_proc_type)WriteRequest, 0, (void*)requestArgs);

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
};


// Create a list of arguments needed for the callback
LPREQUEST_DATA CreateNewRequestArgs(WPARAM wParam){

    // We allocate these in the heap so they persist for threads to use
    LPREQUEST_DATA requestArgs = (LPREQUEST_DATA)malloc(sizeof(REQUEST_DATA));
    SOCKET* Accept = (SOCKET*)malloc(sizeof(SOCKET));
    LPSOCKET_INFORMATION SocketInfo = (LPSOCKET_INFORMATION)malloc(sizeof(LPSOCKET_INFORMATION)); // #todo /!\ peut poser souci (remplacer par SOCKET_INFORMATION dans le SIZEOF)
    DWORD RecvBytes;
    DWORD SendBytes;
    DWORD Flags;

    requestArgs->Accept = Accept;
    requestArgs->SocketInfo = SocketInfo;
    requestArgs->RecvBytes = RecvBytes;
    requestArgs->SendBytes = SendBytes;
    requestArgs->Flags = Flags;
    requestArgs->wParam = wParam;
    return requestArgs;
}

void FreeRequestArgs(LPREQUEST_DATA requestArgs) {
    free(requestArgs->Accept);
    free(requestArgs->SocketInfo);
    free(requestArgs);
}


//int ReadRequest(LPSOCKET_INFORMATION* SocketInfo, WPARAM* wParam, DWORD* RecvBytes, DWORD* Flags) {
int ReadRequest(LPREQUEST_DATA requestArgs) {
    requestArgs->SocketInfo = GetSocketInformation(requestArgs->wParam);
    // Read data only if the receive buffer is empty
    if (requestArgs->SocketInfo == NULL) {
        return -1; // if return -1 : break
    }
    if (requestArgs->SocketInfo->BytesRECV != 0)
    {
        requestArgs->SocketInfo->RecvPosted = TRUE;
        return 0;
    }
    else
    {
        requestArgs->SocketInfo->DataBuf.buf = requestArgs->SocketInfo->Buffer;
        requestArgs->SocketInfo->DataBuf.len = DEFAULT_HEADERSIZE;
        requestArgs->Flags = 0;
        if (WSARecv(requestArgs->SocketInfo->Socket, &(requestArgs->SocketInfo->DataBuf), 1, &requestArgs->RecvBytes,
            &requestArgs->Flags, NULL, NULL) == SOCKET_ERROR)
        {
            if (WSAGetLastError() != WSAEWOULDBLOCK)
            {
                printf("WSARecv() failed with error %d\n", WSAGetLastError());
                FreeSocketInformation(requestArgs->wParam);
                return 0;
            }
        }
        else // No error so update the byte count
        {
            printf("WSARecv() is OK!\n");

            // Char* to int converter
            int dataBufferSize = 0;
            for (int i = 0; i < DEFAULT_HEADERSIZE; i++)
            {
                printf("   /!\\ Entering for() loop ! Iteration number %d", i + 1);
                dataBufferSize |= requestArgs->SocketInfo->DataBuf.buf[i] << i * 8;
                printf("      dataBufferSize is now %08X\n", dataBufferSize);
            }

            requestArgs->SocketInfo->DataBuf.buf = requestArgs->SocketInfo->Buffer;
            requestArgs->SocketInfo->DataBuf.len = dataBufferSize;
            requestArgs->Flags = 0;
            if (WSARecv(requestArgs->SocketInfo->Socket, &(requestArgs->SocketInfo->DataBuf), 1, &requestArgs->RecvBytes,
                &requestArgs->Flags, NULL, NULL) == SOCKET_ERROR)
            {
                if (WSAGetLastError() != WSAEWOULDBLOCK)
                {
                    printf("WSARecv() failed with error %d\n", WSAGetLastError());
                    FreeSocketInformation(requestArgs->wParam);
                    FreeRequestArgs(requestArgs);
                    return 0;
                }
            }
            else // No error so update the byte count
            {
                /*
                 *  Convertion in the other direction because server is f* cked
                 */

                 // Convertion into std::string so that the Json library can use it
                char* recievedData = requestArgs->SocketInfo->DataBuf.buf;
                std::string json_data(recievedData);

                // Parsing the Json data
                Json::Value recieveRoot;
                JSONCPP_STRING err;
                Json::CharReaderBuilder readBuilder;
                const std::unique_ptr<Json::CharReader> reader(readBuilder.newCharReader());

                if (!reader->parse(json_data.c_str(), json_data.c_str() + json_data.length(), &recieveRoot, &err))
                {
                    std::cout << "error" << std::endl;
                    return 0;
                }

                requestArgs->SocketInfo->lastJsonValue = recieveRoot;
            }
        }
    }
}


int WriteRequest(LPREQUEST_DATA requestArgs) {
    requestArgs->SocketInfo = GetSocketInformation(requestArgs->wParam);
    if (requestArgs->SocketInfo->BytesRECV > requestArgs->SocketInfo->BytesSEND)
    {
        requestArgs->SocketInfo->DataBuf.buf = requestArgs->SocketInfo->Buffer + requestArgs->SocketInfo->BytesSEND;
        requestArgs->SocketInfo->DataBuf.len = requestArgs->SocketInfo->BytesRECV - requestArgs->SocketInfo->BytesSEND;
        if (WSASend(requestArgs->SocketInfo->Socket, &(requestArgs->SocketInfo->DataBuf), 1, &(requestArgs->SendBytes), 0,
            NULL, NULL) == SOCKET_ERROR)
        {
            if (WSAGetLastError() != WSAEWOULDBLOCK)
            {
                printf("WSASend() failed with error %d\n", WSAGetLastError());
                FreeSocketInformation(requestArgs->wParam);
                return 0;
            }
        }
        else // No error so update the byte count
        {
            printf("WSASend() is OK!\n");
            requestArgs->SocketInfo->BytesSEND += requestArgs->SendBytes;
        }
    }
    if (requestArgs->SocketInfo->BytesSEND == requestArgs->SocketInfo->BytesRECV)
    {
        requestArgs->SocketInfo->BytesSEND = 0;
        requestArgs->SocketInfo->BytesRECV = 0;
        // If a RECV occurred during our SENDs then we need to post an FD_READ notification on the socket
        if (requestArgs->SocketInfo->RecvPosted == TRUE)
        {
            requestArgs->SocketInfo->RecvPosted = FALSE;

            //PostMessage(hwnd, WM_SOCKET, wParam, FD_READ);
        }
    }
    //PostMessage(hwnd, WM_SOCKET, wParam, FD_WRITE);
    std::cout << "hey there" << std::endl;
    return -1;
}