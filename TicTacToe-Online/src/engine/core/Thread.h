#pragma once

#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define NOMINMAX

#include <winsock2.h>
#include <windows.h>
#include <vector>

class Thread
{
public:
	Thread() {};
	~Thread() {};

	static void EnterThread(void* threadAdress);
	void Run();
	virtual int InitThread() = 0;
	virtual int MainLoop() = 0;
	void CleanUp() {};
};