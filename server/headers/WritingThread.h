#pragma once

#include "Thread.h"

class WritingThread : public Thread
{
public:
	WritingThread();
	~WritingThread();


	void RunThread();
};