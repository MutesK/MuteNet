#pragma once

#include "MuteNetFoundation.h"
#include "TcpSocket.h"

/*
 *  Accept 되면 해당 Link(Session)에 EndPoint(접속 정보) 등 셋팅후, IOEngine으로 콜백 -> IOCP 핸들 등록 후 유저 콜백
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
