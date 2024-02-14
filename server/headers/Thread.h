#pragma once

#include <winsock2.h>
#include <Windows.h>

class Thread 
{
public:

	static void SummonThread(void* functionAdress);
	static std::vector<HANDLE> threadList;

	virtual void RunThread() {};
	

	WSADATA m_wsaData;
	HANDLE m_This;
};