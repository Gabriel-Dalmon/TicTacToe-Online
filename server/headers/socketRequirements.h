#pragma once

#include <json/json.h>
#include <winsock2.h>
#include <Windows.h>

// forward declarations
namespace std {
    template<typename T, typename U>
    class pair;
}



// gloabal macro
#define WIN32_LEAN_AND_MEAN

#define WM_SOCKET                               (WM_USER + 1)
#define PORT                                    5150
#define DATA_BUFSIZE                            8192
#define DEFAULT_HEADERSIZE                        4

enum matchStates
{
    waiting_players = 0,
    running,
    player1Win,
    player2Win,
    tie
};

enum requestType
{
    IGNORE_REQ = 0,     // Ignore and never send the request
    setName,            // Contains the new name for a player
    updateOpponentName, // Contains the new name of the opponent (to update on the client's screen)
    makePlay,           // Contains a player's index where they want to play
    updateMatchState    // Contains all the info about the match. Sent after a legal move is played
};

// typedef definition
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
