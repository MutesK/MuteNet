#pragma once

#include <WinSock2.h>
#include <memory>
#include "SocketAddress.h"

enum class ShutdownBlockMode
{
	RecvBlock = SD_RECEIVE,
	SendBlock = SD_SEND,
	BothBlock = SD_BOTH,
};
class Socket
{
protected:
	Socket(ADDRESS_FAMILY);
public:
	virtual ~Socket();

	virtual bool bind(const std::shared_ptr<SocketAddress>& address) = 0;

	// false : Block IO Mode
	// true : Non-Block IO Mode
	int setIOMode(bool ioflag);

	int getLastError();

	int setRecvSocketBufferSize(int size);
	int setSendSocketBufferSize(int size);

	int setReUseAddress(bool toggle);

	int setRecvCallLimitTime(DWORD time);
	int setSendCallLimitTime(DWORD time);

	int setUseKeepAlive(bool toggle);
	int shutdown(ShutdownBlockMode Mode);

	GET_SET_ATTRIBUTE(SOCKET, handle);

	friend class SocketUtil;

protected:
	SOCKET			_handle;
	DWORD			_lastError;
	ADDRESS_FAMILY	_address_family;
};

