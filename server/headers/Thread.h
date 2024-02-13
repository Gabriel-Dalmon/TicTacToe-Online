#pragma once

#include <winsock2.h>
#include <Windows.h>

class Thread 
{
public:
	Thread(LPWSADATA wsaDataAddr);
	~Thread();


	static void SummonThread(void* functionAdress);
	static std::vector<HANDLE> threadList;


	void RunSocketThread();
	void CloseSocket();
	

	LPWSADATA m_wsaDataAddr;
	HANDLE m_This;
};