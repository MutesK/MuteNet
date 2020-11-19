//
// Created by junmkim on 2020-11-13.
//

#ifndef MUTENET_DESCRIPTOR_H
#define MUTENET_DESCRIPTOR_H

#include "TypeDefine.hpp"
#include "EventBaseComponent.hpp"
#include <CircularBuffer.h>
#include <InputMemoryStream.h>

namespace EventLoop
{
	class IDescriptor;
	
	using CallbackPtr = void ( * ) (IDescriptor *, void *Self );
	using ExceptCallackPtr = void ( * ) (IDescriptor *, uint16_t What, void *Self );
	
	using CircularBufferWithLock = LockObject<Util::CircularBuffer>;
	
	class IDescriptor : public IEventBaseComponent
	{
	protected:
		descriptor_t _descriptor;
		
		CallbackPtr _ReadCallback = nullptr;
        CallbackPtr _WriteCallback = nullptr;
        ExceptCallackPtr _ExceptCallback = nullptr;
        void *_Key = nullptr;

        CircularBufferWithLock _ReadBuffer;
        CircularBufferWithLock _WriteBuffer;

        friend class IOContextImpl;

        friend class SelectIOContext;

        friend class EpollContextImpl;

        friend class KQueueIOContextImpl;

        IDescriptor(const RawIOContextImplPtr &Ptr, descriptor_t descriptor);

    public:
        virtual ~IDescriptor();

        virtual void Read() = 0;

        virtual void Write(void *data, size_t length) = 0;

		virtual void Shutdown (uint16_t Flag ) = 0;
		
		void SetCallback ( CallbackPtr ReadCallback, CallbackPtr WriteCallback,
		                   ExceptCallackPtr ExceptionCallback, void *Key );
		
		descriptor_t GetDescriptor ( ) const;

        Util::InputMemoryStream GetReadBuffer() const;
        Util::CircularBuffer GetWriteBuffer() const;

        bool IsVaildCallback() const;

	protected:
        virtual void Enable() = 0;

		virtual bool _Read();
		
		virtual bool _Write();

#ifdef WIN32
		// Windows doesn't support the POSIX Descriptor Common IO function.
		virtual int write(descriptor_t descriptor, const char* ptr, size_t length) = 0;

		virtual int read(descriptor_t descriptor, char* ptr, size_t length) = 0;
#endif
	};
}


#endif //MUTENET_DESCRIPTOR_H
