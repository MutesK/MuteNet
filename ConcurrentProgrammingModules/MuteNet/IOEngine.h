#pragma once

#include "Link.h"
#include "Acceptor.h"
#include "IOService.h"

namespace Network
{
	using Event = std::function<void(Link::Handle, void*)>;

	/*
	 *  유저 영역에서는 Connector, Acceptor, IOEngine 만 컨트롤링 가능하며, 그마저도 간접적으로만 가능하게 한다.
	 */
	class IOEngine final
	{
	private:
		std::pair<Event, void*> _Accepted;
		std::pair<Event, void*> _Disconnected;
		std::pair<Event, void*> _Connected;
		std::pair<Event, void*> _Recived;
		std::pair<Event, void*> _Sended;

		Acceptor _Acceptor;
		IOService _IOService;
	private:
		NON_COPYABLE(IOEngine);
	public:
		IOEngine();
		~IOEngine();

		void SetOnAccepted( Network::Event& callback, void *Context);
		void SetOnDisconnect( Network::Event& callback, void* Context);
		void SetOnConnected( Network::Event& callback, void* Context);
		void SetOnRecived( Network::Event& callback, void* Context);
		void SetOnSended( Network::Event& callback, void* Context);

		void StartServer(const std::string ip, uint16_t port);
		void StartClient(const std::string ip, uint16_t port);


		void ForceStop();
		void SafetyStop();

	private:
		void InterAccept(LinkPtr Link, void* Context);
		void InterConnected(LinkPtr Link);
		void InterDisconnected(LinkPtr Link);
		void InterRecived(LinkPtr Link);
		void InterSended(LinkPtr Link);

		friend class Acceptor;
	};


	inline void IOEngine::SetOnAccepted( Network::Event& callback, void* Context)
	{
		_Accepted = { std::move(callback), Context };
	}
	
	inline void IOEngine::SetOnDisconnect( Network::Event& callback, void* Context)
	{
		_Disconnected = { std::move(callback), Context };
	}

	inline void IOEngine::SetOnConnected( Network::Event& callback, void* Context)
	{
		_Connected = { std::move(callback), Context };
	}

	inline void IOEngine::SetOnRecived( Network::Event& callback, void* Context)
	{
		_Recived = { std::move(callback), Context };
	}

	inline void IOEngine::SetOnSended( Network::Event& callback, void* Context)
	{
		_Sended = { std::move(callback), Context };
	}

	inline void IOEngine::InterAccept(LinkPtr Link, void* Context)
	{
		Link->AddRef();
		IOEngine::_Accepted.first(Link.GetRawPtr(), IOEngine::_Accepted.second);
		Link->Release();

		_IOService.RegisterSocket(Context, Link);
	}

	inline void IOEngine::InterConnected(LinkPtr Link)
	{
		Link->AddRef();
		IOEngine::_Connected.first(Link.GetRawPtr(),IOEngine::_Connected.second);
		Link->Release();
	}

	inline void IOEngine::InterDisconnected(LinkPtr Link)
	{
		Link->AddRef();
		IOEngine::_Disconnected.first(Link.GetRawPtr(), IOEngine::_Disconnected.second);
		Link->Release();
	}

	inline void IOEngine::InterRecived(LinkPtr Link)
	{
		Link->AddRef();
		IOEngine::_Recived.first(Link.GetRawPtr(), IOEngine::_Recived.second);
		Link->Release();
	}

	inline void IOEngine::InterSended(LinkPtr Link)
	{
		Link->AddRef();
		IOEngine::_Sended.first(Link.GetRawPtr(), IOEngine::_Sended.second);
		Link->Release();
	}

}
