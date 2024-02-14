#pragma once

#include <winsock2.h>
#include <Windows.h>

class Thread 
{
public:

	static void SummonThread(void* functionAdress);
	static std::vector<HANDLE> threadList;


<<<<<<< HEAD

	void RunSocketThread();
=======
	virtual void RunThread() {};
>>>>>>> d57bcc6fa3c9428c41c47c39ea4159ceee3b3ab6
	

	WSADATA m_wsaData;
	HANDLE m_This;
};