#pragma once

#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define NOMINMAX

#include <winsock2.h>
#include <windows.h>
#include <queue>

#include <json/json.h>

#define MAX_BUFFER_SIZE 8192
#define HEADER_SIZE 4

#define SERVER_IP ""
#define LOCAL_IP "127.0.0.1"
#define DEFAULT_PORT 5150

#define WM_SOCKET (WM_USER + 1)

class Socket;

struct CallbackData {
	Socket* instance;
	void (Socket::* methodPointer)(HANDLE arguments);
	void* arguments;
};

class Socket
{
public:
	Socket(HWND hwnd);
	~Socket();
	void Connect(const char* cServerIp, int iPort);
	void SendJSON(int requestType, Json::Value JSONRequestData);
	void DeleteBuffer(void* pFullSendBuffer);
	void Close();

	// WinProc Socket Callbacks - Called on FD_CONNECT/FD_READ/FD_WRITE/FD_CLOSE
	int OnConnect();
	int OnReceive();
	void OnHeaderReceived(void* rawHeaderBuffer);
	void OnDataReceived(void* rawDataBuffer);
	int OnSend();
	int OnDisconnect();

	inline SOCKET GetWinSocket() { return m_winSocket; }
	static void CALLBACK WSAAsyncCallbacksHandler(DWORD dwError, DWORD cbTransferred, LPWSAOVERLAPPED lpOverlapped, DWORD dwFlags);

private:
	LPWSAOVERLAPPED GetWSAOverlappedStruct(void (Socket::* methodPointer)(HANDLE arguments), void* arguments);
	SOCKET m_winSocket;
	SOCKADDR_IN m_addressInfo;
	std::queue<LPWSABUF> m_sendQueue;
	HWND m_HWND;
};