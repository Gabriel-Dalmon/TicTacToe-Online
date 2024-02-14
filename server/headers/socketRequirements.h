#pragma once

#include <json/json.h>
#include <winsock2.h>
#include <Windows.h>



// gloabal macro
#define WIN32_LEAN_AND_MEAN

#define WM_SOCKET                               (WM_USER + 1)
#define PORT                                    5150
#define DATA_BUFSIZE                            8192
#define DEFAULT_HEADERSIZE                        4

enum RequestType
{
    setName = 0,
    makePlay
};

// struct definition
typedef struct _SOCKET_INFORMATION {
    BOOL RecvPosted;
    CHAR Buffer[DATA_BUFSIZE];
    WSABUF DataBuf;
    SOCKET Socket;
    DWORD BytesSEND;
    DWORD BytesRECV;
    struct _SOCKET_INFORMATION* Next;
    Json::Value lastJsonValue;
} SOCKET_INFORMATION, * LPSOCKET_INFORMATION;


// prototypes
HWND MakeWorkerWindow(void);
void CreateSocketInformation(SOCKET s);
LPSOCKET_INFORMATION GetSocketInformation(SOCKET s);
void FreeSocketInformation(SOCKET s);
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
std::pair<SOCKET, HWND> WindowSocketInitialize(WSADATA* wsadata);