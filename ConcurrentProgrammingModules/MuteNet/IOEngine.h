#pragma once

#include "Link.h"
#include "Acceptor.h"
#include "IOService.h"

namespace Network
{
	using Event = std::function<void(Link::Handle, void*)>;

	/*
	 *  ���� ���������� Connector, Acceptor, IOEngine �� ��Ʈ�Ѹ� �����ϸ�, �׸����� ���������θ� �����ϰ� �Ѵ�.
	 */
	class IOEngine final
	{ 
	private:
		std::pair<Event, void*> _Accepted;
		std::pair<Event, void*> _Disconnected;
		std::pair<Event, void*> _Connected;
		std::pair<Event, void*> _Recived;
		std::pair<Event, void*> _Sended;



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

		void ForceStop();
		void SafetyStop();

	private:
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


}
