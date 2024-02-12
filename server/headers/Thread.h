#pragma once

#include <winsock2.h>
#include <Windows.h>

class Thread 
{
public:
	Thread();
	~Thread();


	static void SummonThread(void* functionAdress);

	void launchSocketThread(LPWSADATA wsaData);

	void SocketThread();
	
	static HANDLE threadList[8];
	static int threadNumber;

	LPWSADATA m_wsaDataAddr;
	int m_ID;
};