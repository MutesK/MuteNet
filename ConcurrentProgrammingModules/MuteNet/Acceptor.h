#pragma once

#include "MuteNetFoundation.h"
#include "TcpSocket.h"

/*
 *  Accept �Ǹ� �ش� Link(Session)�� EndPoint(���� ����) �� ������, IOEngine���� �ݹ� -> IOCP �ڵ� ��� �� ���� �ݹ�
 */
namespace Network
{
	class Acceptor final
	{
	private:
		std::unique_ptr<TcpSocket> _Listen;
		std::vector<std::thread> _threads;
		
	public:
		Acceptor();
		~Acceptor() = default;

		bool Listen(const std::string& ip, uint16_t port, uint16_t maxthread);
		
		void Start();
		void Stop();
	private:
		void AcceptorWork();
	};
}
