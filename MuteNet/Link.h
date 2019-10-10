#pragma once

#include "Socket.h"
#include "TcpSocket.h"
#include "IOContext.h"
#include "Network.h"

namespace MuteNet
{
	class Link
	{
		friend class Network;
	public:
		class Callbacks
		{
		public:
			virtual ~Callbacks() {}

			virtual void OnCreated(Link* Link) = 0;

			virtual void OnReceivedData(const char* data, size_t Length) = 0;

			virtual void OnRemoteClosed(void) = 0;

			virtual void OnTLSHandShakeCompleted() = 0;

			virtual void OnError(int ErrorCode, const std::string& ErrorMsg) = 0;
		};

		typedef std::shared_ptr<Callbacks> CallbacksPtr;
	protected:
		CallbacksPtr _Callback;

		Link(CallbacksPtr callbacks)
			:_Callback(callbacks)
		{
		}

		Network::ConnectCallbacksPtr _ConnectCallbacks;
	public:
		virtual ~Link();

		virtual bool Send(const void* Data, size_t Length) = 0;

		virtual std::string GetLocalIP(void) const = 0;

		virtual uint16_t GetLocalPort() const = 0;

		virtual std::string GetRemoteIP() const = 0;

		virtual uint16_t GetRemotePort() const = 0;

		virtual void Shutdown() = 0;
		virtual void Close() = 0;
	};
}
