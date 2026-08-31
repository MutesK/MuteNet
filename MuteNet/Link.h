#pragma once

#include <memory>
#include <string>

namespace MuteNet
{
	enum class ProtocolType
	{
		TCP,
		UDP
	};

	class ALink
	{
	public:
		class Callbacks
		{
		public:
			virtual ~Callbacks() = default;

			virtual void OnCreated(ALink* link) = 0;
			virtual void OnReceived(ALink* link, const char* data, size_t length) = 0;
			virtual void OnRemoteClosed(ALink* link) = 0;
			virtual void OnTLSHandshakeCompleted(ALink* link) = 0;
			virtual void OnError(ALink* link, int errorCode, const std::string& errormsg) = 0;
		};
		using CallbacksPtr = std::shared_ptr<Callbacks>;

	protected:
		CallbacksPtr callbacks;

		explicit ALink(CallbacksPtr cb) 
			: callbacks(std::move(cb)) {}

	public:
		virtual ~ALink() = default;

		virtual bool Send(const char* data, size_t length) = 0;
		
		virtual std::string GetLocalIP() const = 0;
		virtual uint16_t GetLocalPort() const = 0;
		virtual std::string GetRemoteIP() const = 0;
		virtual uint16_t GetRemotePort() const = 0;

		virtual CallbacksPtr GetCallbacks() const = 0;
		virtual ProtocolType GetProtocolType() const = 0;

		virtual void Shutdown() = 0;
		virtual void Close() = 0;
	};

	using LinkPtr = std::shared_ptr<ALink>;
}