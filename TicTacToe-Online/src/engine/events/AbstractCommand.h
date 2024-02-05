#pragma once

#include "../../utils/VoidClass.h"

struct EventContext {};

struct UniqueCommandIdentifier {
	void* methodInstancePointer;
	void(VoidClass::* methodPointer)();
	void(*functionPointer)();

	UniqueCommandIdentifier(void* methodInstancePtr, void (VoidClass::* methodPtr)(), void (*funcPtr)()) {
		methodInstancePointer = methodInstancePtr;
		methodPointer = methodPtr;
		functionPointer = funcPtr;
	}
};

class AbstractCommand {
public:
	AbstractCommand() {};
	~AbstractCommand() {};
	virtual int execute(EventContext* context) = 0;
	virtual bool compareCommandsIdentifier(UniqueCommandIdentifier* commandIdentifier) = 0;
	UniqueCommandIdentifier* commandIdentifier;
private:
};
