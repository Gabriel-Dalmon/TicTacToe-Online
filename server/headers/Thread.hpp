#pragma once

class Thread 
{
public:
	Thread();
	~Thread();


	static HANDLE SummonThread(void* functionAdress);

	void launchSocketThread(LPWSADATA wsaData);

	void SocketThread();
	
	static HANDLE threadList[8];
	static int threadNumber;

	LPWSADATA m_wsaDataAddr;
	int m_ID;
};