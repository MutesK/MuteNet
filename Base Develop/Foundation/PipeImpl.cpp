#include "PipeImpl.h"
#include "Exception.h"

PipeImpl::PipeImpl()
{
	SECURITY_ATTRIBUTES security_attr;
	security_attr.nLength = sizeof(security_attr);
	security_attr.lpSecurityDescriptor = nullptr;
	security_attr.bInheritHandle = false;

	if (false == CreatePipe(&_readHandle, &_writeHandle, &security_attr,
		0))
		throw CFileException("Create Pipe Failed");

}


PipeImpl::~PipeImpl()
{
	closePipe();
}

int PipeImpl::SendBytes(const char* buff, int length)
{
	// assert to write handle

	DWORD bytes = 0;
	if (false == WriteFile(_writeHandle, buff, length, &bytes, nullptr))
	{
		// error;
	}

	return bytes;
}


int PipeImpl::RecvBytes(char* buffer, int length)
{
	// assert to write handle

	DWORD bytes = 0;
	BOOL result = ReadFile(_readHandle, buffer, length, &bytes, nullptr);

	if (result == true || GetLastError() == ERROR_BROKEN_PIPE)
		return bytes;
	else
	{
		// Error
		return bytes;
	}
}

void PipeImpl::closePipe()
{
	CloseHandle(_readHandle);
	CloseHandle(_writeHandle);

	_readHandle = INVALID_HANDLE_VALUE;
	_writeHandle = INVALID_HANDLE_VALUE;
}

HANDLE PipeImpl::readHandle() const
{
	return _readHandle;
}

HANDLE PipeImpl::writeHandle() const
{
	return _writeHandle;
}