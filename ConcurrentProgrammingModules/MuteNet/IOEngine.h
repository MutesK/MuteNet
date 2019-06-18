#pragma once

#include "Link.h"
#include "Acceptor.h"

namespace Network
{
	using Event = std::function<void(LinkPtr, void*)>;

	/*
	 *  유저 영역에서는 Connector, Acceptor, IOEngine 만 컨트롤링 가능하며, 그마저도 간접적으로만 가능하게 한다.
	 */
	class IOEngine final
	{
	private:
		static std::pair<Event, void*> _Accepted;
		static std::pair<Event, void*> _Disconnected;
		static std::pair<Event, void*> _Connected;
		static std::pair<Event, void*> _Recived;
		static std::pair<Event, void*> _Sended;

		Acceptor _Acceptor;
	private:
		NON_COPYABLE(IOEngine);
	public:
		IOEngine();
		~IOEngine();

		static void SetOnAccepted( Network::Event& callback, void *Context);
		static void SetOnDisconnect( Network::Event& callback, void* Context);
		static void SetOnConnected( Network::Event& callback, void* Context);
		static void SetOnRecived( Network::Event& callback, void* Context);
		static void SetOnSended( Network::Event& callback, void* Context);

		void Start(const std::string ip, uint16_t port);
		void Start(const std::string connect);

		void ForceStop();
		void SafetyStop();

	private:
		static void InterAccept(LinkPtr Link);
		static void InterConnected(LinkPtr Link);
		static void InterDisconnected(LinkPtr Link);
		static void InterRecived(LinkPtr Link);
		static void InterSended(LinkPtr Link);

		friend class Acceptor;
	};


	inline void IOEngine::SetOnAccepted( Network::Event& callback, void* Context)
	{
		IOEngine::_Accepted = { std::move(callback), Context };
	}
	
	inline void IOEngine::SetOnDisconnect( Network::Event& callback, void* Context)
	{
		IOEngine::_Disconnected = { std::move(callback), Context };
	}

	inline void IOEngine::SetOnConnected( Network::Event& callback, void* Context)
	{
		IOEngine::_Connected = { std::move(callback), Context };
	}

	inline void IOEngine::SetOnRecived( Network::Event& callback, void* Context)
	{
		IOEngine::_Recived = { std::move(callback), Context };
	}

	inline void IOEngine::SetOnSended( Network::Event& callback, void* Context)
	{
		IOEngine::_Sended = { std::move(callback), Context };
	}

	inline void IOEngine::InterAccept(LinkPtr Link)
	{
		IOEngine::_Accepted.first(Link, IOEngine::_Accepted.second);
	}

	inline void IOEngine::InterConnected(LinkPtr Link)
	{
		IOEngine::_Connected.first(Link, IOEngine::_Connected.second);
	}

	inline void IOEngine::InterDisconnected(LinkPtr Link)
	{
		IOEngine::_Disconnected.first(Link, IOEngine::_Disconnected.second);
	}

	inline void IOEngine::InterRecived(LinkPtr Link)
	{
		IOEngine::_Recived.first(Link, IOEngine::_Recived.second);
	}

	inline void IOEngine::InterSended(LinkPtr Link)
	{
		IOEngine::_Sended.first(Link, IOEngine::_Sended.second);
	}

}
