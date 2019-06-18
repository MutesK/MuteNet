#pragma once

#include "MuteNetFoundation.h"
#include "TcpSocket.h"
#include "EndPoint.h"
#include <thread>
#include "Link.h"

/*
 *  Accept �Ǹ� �ش� Link(Session)�� EndPoint(���� ����) �� ������, IOEngine���� �ݹ� -> IOCP �ڵ� ��� �� ���� �ݹ�
 *  
 *  ���� ������ ��Ʈ�� ����
 */
namespace Network
{
	class Acceptor final
	{
	public:
		using Event = std::function<void(LinkPtr, void*)>;
	private:
		std::unique_ptr<TcpSocket> _Listen;
		std::vector<std::thread*> _threadpool;

		Event _OnAccept;
	public:
		Acceptor();
		~Acceptor() = default;

		bool Listen(const std::string& ip, uint16_t port, uint16_t maxthread);
		
		void Start();
		void Stop();

		void SetOnAccept(const Event& callback);
	private:
		void AcceptorWork();
	};
}
