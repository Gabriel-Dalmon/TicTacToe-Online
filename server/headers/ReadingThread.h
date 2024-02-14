#pragma once

#include "Thread.h"
#include <mutex>

class ReadingThread : public Thread
{
public:
	ReadingThread();
	~ReadingThread();


	void RunThread();

private:
	WSAData m_wsaData;
	static std::mutex initMutex;	// Necessary for WSA startup and clean-up
};