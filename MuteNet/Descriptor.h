//
// Created by junmkim on 2020-11-13.
//

#ifndef MUTENET_DESCRIPTOR_H
#define MUTENET_DESCRIPTOR_H

#include "EventBaseComponent.hpp"
#include <CircularBuffer.h>
#include <AtomicCounter.hpp>
#include <InputMemoryStream.h>

namespace EventLoop
{
	class IDescriptor;
	
	using CallbackPtr = void ( * ) (IDescriptor *, void *Self );
	using ExceptCallackPtr = void ( * ) (IDescriptor *, uint16_t What, void *Self );
	
	using CircularBufferWithLock = LockObject<Util::CircularBuffer>;
	
	class IDescriptor : public IEventBaseComponent, public Util::AtomicCounter
	{
	protected:
		socket_t _socket;
		
		CallbackPtr _ReadCallback = nullptr;
		CallbackPtr _WriteCallback = nullptr;
		ExceptCallackPtr _ExceptCallback = nullptr;
		void *_Key = nullptr;
		
		CircularBufferWithLock _ReadBuffer;
		CircularBufferWithLock _WriteBuffer;
		
		friend class IOContextImpl;
		friend class SelectIOContext;
        friend class EpollContextImpl;
		
		IDescriptor (const RawIOContextImplPtr &Ptr, socket_t Socket );
	
	public:
		virtual ~IDescriptor();
		
		virtual void Read ( ) = 0;
		
		virtual void Write ( void *data, size_t length ) = 0;
		
		virtual void Enable ( ) = 0;
		
		virtual void Disable ( uint16_t Flag ) = 0;
		
		void SetCallback ( CallbackPtr ReadCallback, CallbackPtr WriteCallback,
		                   ExceptCallackPtr ExceptionCallback, void *Key );
		
		socket_t GetFD ( ) const;
		
		Util::InputMemoryStream GetReadBuffer() const;

        bool IsVaildCallback() const;

	protected:
		bool _Read();
		
		bool _Write();


	};
}


#endif //MUTENET_DESCRIPTOR_H
