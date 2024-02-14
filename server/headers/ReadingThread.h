#pragma once

#include "Thread.h"

class ReadingThread : public Thread
{
public:
	ReadingThread();
	~ReadingThread();


	void RunThread();
};