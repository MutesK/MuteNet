#pragma once

#include "IIODispatcher.h"
#if defined(_WIN32) || defined(_WIN64)
#include "WinIODispatcherImpl.h"
#elif defined(__linux__)
#include "EpollIODispatcherImpl.h"
#elif defined(__APPLE__) || defined(__FreeBSD__)
#include "KQueueIODispatcherImpl.h" 
#endif

namespace MuteNet
{
	class ASyncIORequest
	{
	protected:
#if defined(_WIN32) || defined(_WIN64)
		OverlappedContext osContext{};
#elif defined(__linux__)
		EpollAsyncContext osContext{};
#elif defined(__APPLE__) || defined(__FreeBSD__)
		KqueueAsyncContext osContext{};
#endif

	public:
		ASyncIORequest()
		{
#if defined(_WIN32) || defined(_WIN64)
			std::memset(&osContext, 0, sizeof(OverlappedContext));
			osContext.Context = this;
#elif defined(__linux__)
			std::memset(&osContext, 0, sizeof(EpollAsyncContext));
			osContext.customContext = this;
#elif defined(__APPLE__) || defined(__FreeBSD__)
			std::memset(&osContext, 0, sizeof(KqueueAsyncContext));
			osContext.customContext = this;
#endif
		}

		virtual ~ASyncIORequest() = default;

		void* GetPlatformContext()
		{
			return &osContext;
		}

		// 파생 클래스에서 구현할 순수 가상 함수들
		virtual bool Process() = 0;
		virtual void OnComplete(uint32_t transferredBytes, IOOperation op) = 0;
		virtual void OnError(IOOperation op) = 0;
	};
}