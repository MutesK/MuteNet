//
// Created by Mute on 2020-11-14.
//

#ifndef MUTENET_WINDESCRIPTOR_HPP
#define MUTENET_WINDESCRIPTOR_HPP

#if defined(WIN32)

#include <cstddef>
#include <cstdint>
#include "Descriptor.h"
#include "AtomicCounter.hpp"

namespace EventLoop
{
	class IWinDescriptor
	{
	public:
		virtual void IOCompletion(OVERLAPPED* pRawOverlapped, uint32_t TransfferedBytes) = 0;

		virtual void IOError(OVERLAPPED* pRawOverlapped, uint32_t LastError) = 0;

		virtual void IOTimeout(OVERLAPPED* pRawOverlapped) = 0;
	};

	class WinSocketDescriptor : public IDescriptor, public IWinDescriptor, public Util::AtomicCounter
	{
		OVERLAPPED _RecvOverlapped{};
		OVERLAPPED _SendOverlapped{};

		friend class Win32ListenerComponent;
	protected:
		WinSocketDescriptor(RawIOContextImplPtr const& Ptr, descriptor_t Socket);
		~WinSocketDescriptor();

		virtual int write(descriptor_t descriptor, const char* ptr, size_t length) override;

		virtual int read(descriptor_t descriptor, char* ptr, size_t length) override;

		friend class IocpContextImpl;

	public:
		virtual void Enable() override;

		virtual void Shutdown(uint16_t Flag) override;

		virtual void Write(void* data, size_t length) override;

	private:
		virtual void Read() override;

		virtual void IOCompletion(OVERLAPPED* pRawOverlapped, uint32_t TransfferedBytes) override;

		virtual void IOError(OVERLAPPED* pRawOverlapped, uint32_t LastError) override;

		virtual void IOTimeout(OVERLAPPED* pRawOverlapped) override;
	};
}

#endif


#endif //MUTENET_WINDESCRIPTOR_HPP
