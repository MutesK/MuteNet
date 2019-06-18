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
		_Listen->Bind(ip, port);

		const auto work = std::bind(&Acceptor::AcceptorWork, this);

		for(auto thread_count = 0; thread_count <= maxthread; ++thread_count)
		{
			_threadpool.emplace_back(new std::thread(work));
		}

		for(auto i = 0; i< maxthread; ++i)
		{
			Util::ChangeThreadName(_threadpool[i]->native_handle(), "Acceptor Thread");
		}

		return true;
	}

	void Acceptor::Start()
	{
	}

	void Acceptor::Stop()
	{
		for (auto& thread : _threadpool)
		{
			thread->join();
			delete thread;
		}

		_threadpool.clear();

	}

	void Acceptor::SetOnAccept(const Event& callback)
	{
		_OnAccept = std::move(callback);
	}

	void Acceptor::AcceptorWork()
	{
		while (true)
		{
			const auto socket = _Listen->Accept();

			// User Session Pool -> 임시로 이렇게 처리...
			auto link = new Link();
			link->_socket = socket;
			const LinkPtr ptr(link);

			_OnAccept(ptr, link->native_handle());
		}
	}

}
