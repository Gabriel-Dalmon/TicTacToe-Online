#include <iostream>

#include "SocketThread.h"
#include "Socket.h"

SocketThread::SocketThread()
{
    m_HWND = nullptr;
}

Socket* SocketThread::CreateSocket()
{
    if (m_HWND == nullptr) {
        std::cout << "Couldn't create socket - SocketThread not entirely initialized\n";
        return nullptr;
    }
    Socket* pNewSocket = new Socket(m_HWND);
    m_SocketList.push_back(pNewSocket);
    return pNewSocket;
}

Socket* SocketThread::GetSocketInfo(SOCKET pWinSocket)
{
    for (Socket* pClientSocket : m_SocketList) {
        if (pClientSocket->GetWinSocket() == pWinSocket) {
            return pClientSocket;
        }
    }
    return nullptr;
}

void SocketThread::HandleSocketEvents(SOCKET socket, LPARAM lParam)
{
    Socket* pSocket;
    if ((pSocket = GetSocketInfo(socket)) == nullptr) { return; } //ClientSocket not found;
    switch (WSAGETSELECTEVENT(lParam))
    {
    case FD_CONNECT:
        pSocket->OnConnect();
        break;
    case FD_READ:
        pSocket->OnReceive();
        break;
    case FD_WRITE:
        pSocket->OnSend();
        break;
    case FD_CLOSE:
        pSocket->OnDisconnect();
        break;
    }
}

LRESULT CALLBACK SocketThread::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    SocketThread* pThreadInstance = (SocketThread*)GetProp(hwnd, L"SockThreadAddr");
    if (uMsg == WM_SOCKET)
    {
        pThreadInstance->HandleSocketEvents(wParam, lParam);
    }
    //return GameManager::instance().getWindow()->getOldSFMLWinProc()(hwnd, uMsg, wParam, lParam);
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int SocketThread::InitThread() {
    // Initializing the window 
    if ((m_HWND = MakeWorkerWindow()) == NULL) {
        CleanUp();
        return -1;
    }

    SetProp(m_HWND, L"SockThreadAddr", this); //Should check if '\0' at the end of lpcString is needed
    return 0;
}

int SocketThread::MainLoop()
{
    MSG msg;
    DWORD Ret;
    while (Ret = GetMessage(&msg, NULL, 0, 0))
    {
        if (Ret == -1)
        {
            return 1;
        }

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

HWND SocketThread::MakeWorkerWindow()
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

    if ((Window = CreateWindow(
        (LPCWSTR)ProviderClass,
        L"SocketThreadWindow",
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
        std::cout << "CreateWindow() failed with error" << GetLastError() << "\n";
        return NULL;
    }

    std::cout << "CreateWindow() is OK!\n";
    return Window;
}

