#pragma once

#include <winsock2.h>
#include <Windows.h>

class Thread 
{
public:
	Thread();
	~Thread();


	static void SummonThread(void* functionAdress);
	static std::vector<HANDLE> threadList;


	void RunSocketThread();
	

	WSADATA m_wsaData;
	HANDLE m_This;
};