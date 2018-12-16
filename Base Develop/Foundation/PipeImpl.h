#pragma once

#include "foundation.h"

class PipeImpl
{
public:
	PipeImpl();
	~PipeImpl();

	int SendBytes(const char* buff, int length);
	int RecvBytes(char* buffer, int length);

	HANDLE readHandle() const;
	HANDLE writeHandle() const;

	void closePipe();

private:
	HANDLE _readHandle;
	HANDLE _writeHandle;
};

