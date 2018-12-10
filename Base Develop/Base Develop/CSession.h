#pragma once

#include <memory>


class TCPSocket;
class SocketAddress;

class MemoryStream;
class OutputMemoryStream;
class InputMemoryStream;
/*
	���� �������� �� Ŭ������ ��ӹ޾Ƽ� ó���ϵ��� �����Ѵ�.

*/
class CSession
{
public:
	CSession(std::shared_ptr<TCPSocket>& socket, 
		std::shared_ptr<SocketAddress>& address);
	virtual ~CSession();

	bool Recv();


	GET_SET_ATTRIBUTE(std::shared_ptr<TCPSocket>&, tcpSocket);
	GET_SET_ATTRIBUTE(std::shared_ptr<SocketAddress>&, address);
private:
	std::shared_ptr<TCPSocket> _tcpSocket;
	std::shared_ptr<SocketAddress> _address;

	std::shared_ptr<MemoryStream> _OutputStreamBuffer;
	std::shared_ptr<MemoryStream> _InputStreamBuffer;
};

