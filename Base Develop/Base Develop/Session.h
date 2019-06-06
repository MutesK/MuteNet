#pragma once
/*
	��Ʈ��ũ��Ʈ �κп���, ������ �����ߵ� ������ ��Ÿ��.

	User Content�� ���� �ڵ�� ���� ������ �ȵȴ�.

	- Overlapped ���� ��� ����
		IOCP �Ϸ� �����ÿ�, �ش� Overlapped ���� �޶�����, RingBuffer(ȯ������)�� 
		Index ó���� �ȵɼ� �ִ�.
	- ���� �Ϸ����� ����

	�ݵ�� ������, �Ҹ��� ȣ���� �ؾߵ�.
*/

#include "framework.h"


class Session final
{
public:
	Session(TcpSocketPtr& socket);
	~Session();

	inline LinkHandle getHandle();
	inline uint64_t getLastError();
private:
	void IOCallback(LPOVERLAPPED Overlapped, int ioSize);

	void RecvCompletionProcess(int ioSize);
	void SendCompletionProcess(int ioSize);

	void RecvPost();
private:
	TcpSocketPtr			_socket;

	OVERLAPPED				_sendOverlapped;
	OVERLAPPED				_recvOverlapped;

	std::function<void(LPOVERLAPPED, int)> _IOCallback;

	friend class IOCPManager;
	friend class Acceptor;
	friend class SessionManager;
};

inline LinkHandle Session::getHandle()
{
	return reinterpret_cast<LinkHandle>(_socket->get_handle());
}

inline uint64_t Session::getLastError()
{
	return _socket->getLastError();
}
