#pragma once

#include "ISocket.h"

namespace MuteNet
{
	enum class IOOperation
	{
		Accept, 
		Read,
		ReadFrom,
		Write,
	};

	using IOCallback = std::function<void(bool success, uint32_t bytesTransferred, IOOperation op, void* Context)>;

	class IIODispatcher
	{
	public:
		virtual ~IIODispatcher() = default;

		virtual bool Init(size_t threadCount = 0) = 0;
		virtual void Stop() = 0;

		virtual bool RegisterSocket(SocketHandle handle, void* UserContext) = 0;

		virtual bool PostAsyncRead(SocketHandle handle, void* buffer, size_t length, void* overlappedContext) = 0;
		virtual bool PostAsyncWrite(SocketHandle handle, const void* buffer, size_t length, void* overlappedContext) = 0;

		virtual void SetCallback(IOCallback callback) = 0;
	};
}