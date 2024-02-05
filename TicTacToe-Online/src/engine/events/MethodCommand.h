#pragma once
#include "AbstractCommand.h"
#include "../../utils/VoidClass.h"

template<typename T>
class MethodCommand : public AbstractCommand 
{
public:	
	MethodCommand(T* methodInstancePointer, void(T::* methodPointer)()) {
		this->commandIdentifier = new UniqueCommandIdentifier(
			(void*)methodInstancePointer, 
			(void(VoidClass::*)()) methodPointer, 
			nullptr
		);
		this->methodInstancePointer = methodInstancePointer;
		this->methodPointer = methodPointer;
	};

	~MethodCommand() {}
	virtual int execute(EventContext* context) {
		(this->methodInstancePointer->*methodPointer)();
		return 0;
	};
	virtual bool compareCommandsIdentifier(UniqueCommandIdentifier* commandIdentifier) { return (
		commandIdentifier->methodInstancePointer == this->commandIdentifier->methodInstancePointer 
		&& 
		commandIdentifier->methodPointer == this->commandIdentifier->methodPointer); 
	};

protected:
	T* methodInstancePointer;
	void(T::* methodPointer)();
};