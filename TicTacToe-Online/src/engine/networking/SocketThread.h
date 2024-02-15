#pragma once

#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define NOMINMAX

#include <winsock2.h>
#include <windows.h>
#include <vector>

#include "../core/Thread.h"

class Socket;

class SocketThread : public Thread
{
public:
	SocketThread();
	~SocketThread() {};

	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	Socket* CreateSocket();
	int InitThread() override;
	HWND MakeWorkerWindow();
	int MainLoop() override;
	Socket* GetSocketInfo(SOCKET socket);
	void HandleSocketEvents(WPARAM wParam, LPARAM lParam);
	
	

private:
	HWND m_HWND;
	std::vector<Socket*> m_SocketList;
};