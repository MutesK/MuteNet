#pragma once


class TCPSocket;
class SocketAddress;

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
private:
	std::shared_ptr<TCPSocket> _tcpSocket;
	std::shared_ptr<SocketAddress> _address;
};

