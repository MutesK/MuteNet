#include "Acceptor.h"
#include "Link.h"
#include "IOEngine.h"

namespace Network
{
	Acceptor::Acceptor()
	{
		_Listen = std::make_unique<TcpSocket>(AF_INET);
	}

	bool Acceptor::Listen(const std::string& ip, uint16_t port, uint16_t maxthread)
	{
		_Listen->listen(0);
		_Listen->Bind(SocketAddress(ip, port));

		const auto work = std::bind(&Acceptor::AcceptorWork, this);

		for(auto thread_count = 0; thread_count <= maxthread; ++thread_count)
		{
			_threads.emplace_back(std::thread(work));
		}

		for(auto thread : _threads)
		{
			Util::ChangeThreadName(thread.native_handle(), "Acceptor Thread");
		}


	}

	void Acceptor::Stop()
	{
		for(auto thread : _threads)
		{
			thread.join();
		}
	}

	void Acceptor::AcceptorWork()
	{
		while (true)
		{
			auto socket = _Listen->Accept();

			// User Session Pool -> 임시로 이렇게 처리...
			Link* link = new Link();
			link->_socket = socket;
			LinkPtr ptr(link);

			IOEngine::InterAccept();
		}
	}

}
