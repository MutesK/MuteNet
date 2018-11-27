#pragma once

#include <memory>
#include "MemoryStream.h"
#include "InputMemoryStream.h"
#include "OutputMemoryStream.h"

class TCPSocket;
class SocketAddress;
class MemoryStream;

/*
	���� �������� �� Ŭ������ ��ӹ޾Ƽ� ó���ϵ��� �����Ѵ�.

*/
class CSession
{
public:
	CSession(std::shared_ptr<TCPSocket>& socket, 
		std::shared_ptr<SocketAddress>& address);
	virtual ~CSession();

	GET_SET_ATTRIBUTE(std::shared_ptr<TCPSocket>, tcpSocket);
	GET_SET_ATTRIBUTE(std::shared_ptr<SocketAddress>, address);

	GET_SET_ATTRIBUTE(InputMemoryStream&, inBuffer);
	GET_SET_ATTRIBUTE(OutputMemoryStream&, outBuffer);

private:
	std::shared_ptr<TCPSocket> _tcpSocket;
	std::shared_ptr<SocketAddress> _address;

	InputMemoryStream _inBuffer;
	OutputMemoryStream _outBuffer;
};

