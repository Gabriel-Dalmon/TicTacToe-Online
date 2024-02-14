#pragma once

#include "Thread.h"
#include <mutex>

class WritingThread : public Thread
{
public:
	WritingThread();
	~WritingThread();


	void RunThread();

private:
	WSAData m_wsaData;
	static std::mutex initMutex;	// Necessary for WSA startup and clean-up
};