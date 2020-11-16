//
// Created by junmkim on 2020-11-13.
//

#ifndef MUTENET_SOCKETDESCRIPTOR_H
#define MUTENET_SOCKETDESCRIPTOR_H

#include "EventBaseComponent.hpp"
#include <CircularBuffer.h>
#include <AtomicCounter.hpp>
#include <InputMemoryStream.h>

namespace EventLoop
{
	class ISocketDescriptor;
	
	using CallbackPtr = void ( * ) ( ISocketDescriptor *, void *Self );
	using ExceptCallackPtr = void ( * ) ( ISocketDescriptor *, uint16_t What, void *Self );
	
	using CircularBufferWithLock = LockObject<Util::CircularBuffer>;
	
	class ISocketDescriptor : public IEventBaseComponent, public Util::AtomicCounter
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
		
		ISocketDescriptor ( const RawIOContextImplPtr &Ptr, socket_t Socket );
	
	public:
		virtual ~ISocketDescriptor();
		
		virtual void Read ( ) = 0;
		
		virtual void Write ( void *data, size_t length ) = 0;
		
		virtual void Enable ( ) = 0;
		
		virtual void Disable ( uint16_t Flag ) = 0;
		
		void SetCallback ( CallbackPtr ReadCallback, CallbackPtr WriteCallback,
		                   ExceptCallackPtr ExceptionCallback, void *Key );
		
		socket_t GetFD ( ) const;
		
		Util::InputMemoryStream GetReadBuffer() const;
		
	protected:
		bool _Read();
		
		bool _Send();

		bool IsVaildSocket();
	};
}


#endif //MUTENET_SOCKETDESCRIPTOR_H
