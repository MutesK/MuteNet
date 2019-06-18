#pragma once

#include "MuteNetFoundation.h"
#include "TcpSocket.h"
#include "EndPoint.h"
#include <thread>
#include "Link.h"

/*
 *  Accept 되면 해당 Link(Session)에 EndPoint(접속 정보) 등 셋팅후, IOEngine으로 콜백 -> IOCP 핸들 등록 후 유저 콜백
 *  
 *  조건 변수로 컨트롤 예정
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
