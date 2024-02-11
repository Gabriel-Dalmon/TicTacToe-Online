// forward declaration
namespace std {
    template<typename T, typename U>
    class pair;
}

namespace Json {
    class Value;
}


// gloabal macro
#define WIN32_LEAN_AND_MEAN

#define WM_SOCKET                               (WM_USER + 1)
#define PORT                                    5150
#define DATA_BUFSIZE                            8192
#define DEFAULT_HEADERSIZE                      4
#define MAX_THREADS                             8

enum RequestType
{
    setName = 0,
    makePlay
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


typedef struct _REQUEST_DATA {
    SOCKET* Accept;
    LPSOCKET_INFORMATION SocketInfo;
    DWORD RecvBytes;
    DWORD SendBytes;
    DWORD Flags;
    WPARAM wParam;
}REQUEST_DATA, * LPREQUEST_DATA;


// prototypes
HWND MakeWorkerWindow(void);
void CreateSocketInformation(SOCKET s);
LPSOCKET_INFORMATION GetSocketInformation(SOCKET s);
void FreeSocketInformation(SOCKET s);
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
std::pair<SOCKET, HWND> WindowSocketInitialize(WSADATA* wsadata);
LPREQUEST_DATA CreateNewRequestArgs(WPARAM wParam);
int ReadRequest(LPREQUEST_DATA requestArgs);
