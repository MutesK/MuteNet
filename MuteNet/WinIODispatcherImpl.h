#pragma once

#include "IIODispatcher.h"

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#include <vector>
#include <thread>
#include <atomic>

namespace MuteNet
{
	struct OverlappedContext
	{
		WSAOVERLAPPED Overlapped;
		IOOperation Op;
		WSABUF wsabuf;
		void* Context;
	};

	class WinIODispatcherImpl : public IIODispatcher
	{
		HANDLE IocpHandle = 0;
		std::vector<std::thread> WorkerThreads;
		std::atomic<bool> IsRunning = false;
		IOCallback Callback;

	public:
		~WinIODispatcherImpl() override;

		bool Init(size_t threadCount = 0) override;
		void Stop() override;
		bool RegisterSocket(SocketHandle handle, void* UserContext) override;
		bool PostAsyncRead(SocketHandle handle, void* buffer, size_t length, void* overlappedContext);
		bool PostAsyncWrite(SocketHandle handle, const void* buffer, size_t length, void* overlappedContext) override;
		void SetCallback(IOCallback callback) override;
	private:
		void WorkerThread();
		
	};
}


#endif