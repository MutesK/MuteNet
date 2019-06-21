#pragma once

#include "MuteNetFoundation.h"
#include "TcpSocket.h"

namespace Network
{
	enum class OverlappedType
	{
		eConnected,
		eDisconnected,
		eAccepted,
		eRecived,
		eSended,
	};

	struct IOContext :  public OVERLAPPED
	{
		OverlappedType _Type;
		SOCKET		   _Socket;
	};

	struct AcceptIOContext : public OVERLAPPED
	{
		OverlappedType _Type;
		TcpSocket*     _Socket;
		char		   _buffer[50];
	};

}
