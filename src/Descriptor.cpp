//
// Created by junmkim on 2020-11-13.
//

#include "Common.h"
#include "TypeDefine.hpp"
#include "Descriptor.h"

namespace EventLoop
{
	struct Buffer
	{
		char* BufferPtr;
		uint32_t length;
	};

	IDescriptor::IDescriptor(RawIOContextImplPtr const& Ptr, socket_t Socket)
		: IEventBaseComponent(Ptr), _descriptor(Socket)
	{

	}

	IDescriptor::~IDescriptor()
	{
	}

	void IDescriptor::SetCallback(CallbackPtr ReadCallback, CallbackPtr WriteCallback,
		ExceptCallackPtr ExceptionCallback,
		void* Key)
	{
		_ReadCallback = ReadCallback;
		_WriteCallback = WriteCallback;
		_ExceptCallback = ExceptionCallback;
		_Key = Key;
	}

	socket_t IDescriptor::GetDescriptor() const
	{
		return _descriptor;
	}

	bool IDescriptor::_Read()
	{
		Util::OutputMemoryStream StreamBuffer;
		auto& InputMemoryStream = reinterpret_cast<Util::InputMemoryStream&>(StreamBuffer);

		int ret = read(_descriptor, const_cast<char*>(InputMemoryStream.GetBufferPtr()),
			InputMemoryStream.GetRemainingDataSize());

		if (ret == SOCKET_ERROR)
		{
			if (errno != EWOULDBLOCK)
			{
				_ExceptCallback(this, errno, _Key);
				return false;
			}
		}

		if (ret == 0)
		{
			_ExceptCallback(this, ECONNREFUSED, _Key);
			return false;
		}

		StreamBuffer.MoveWritePosition(ret);

		{
			std::unique_lock<std::shared_mutex> lock(_ReadBuffer._mutex);
			_ReadBuffer.PutData(const_cast<char*>(StreamBuffer.GetBufferPtr()), StreamBuffer.GetLength());
		}

		return true;
	}

	bool IDescriptor::_Write()
	{
		Util::OutputMemoryStream StreamBuffer;
		auto& InputMemoryStream = reinterpret_cast<Util::InputMemoryStream&>(StreamBuffer);

		Buffer Buffer[2];

		{
			std::unique_lock<std::shared_mutex> lock(_WriteBuffer._mutex);

			auto SendSize = _WriteBuffer.GetUseSize();

			if (SendSize <= 0)
				return true;

			_WriteBuffer.GetReadBufferAndLengths(&Buffer[0].BufferPtr, Buffer[0].length,
				&Buffer[1].BufferPtr, Buffer[1].length);

			StreamBuffer.Serialize(Buffer[0].BufferPtr, Buffer[0].length);
			if (Buffer[0].length > SendSize)
			{
				StreamBuffer.Serialize(Buffer[1].BufferPtr, Buffer[1].length);
			}
		}

		int ret = write(_descriptor, InputMemoryStream.GetBufferPtr(),
			InputMemoryStream.GetRemainingDataSize());

		if (ret == SOCKET_ERROR)
		{
			if (errno != EWOULDBLOCK)
			{
				_ExceptCallback(this, errno, _Key);
				return false;
			}
		}

		{
			std::unique_lock<std::shared_mutex> lock(_WriteBuffer._mutex);

			_WriteBuffer.MoveReadPostion(ret);
		}

		return true;

	}

	Util::InputMemoryStream IDescriptor::GetReadBuffer() const
	{
		Util::InputMemoryStream Stream;

		Buffer Buffer[2];

		_ReadBuffer.GetReadBufferAndLengths(&Buffer[0].BufferPtr, Buffer[0].length,
			&Buffer[1].BufferPtr, Buffer[1].length);


		reinterpret_cast<Util::OutputMemoryStream&>(Stream).Serialize(Buffer[0].BufferPtr, Buffer[0].length);

		if (Buffer[0].length < _ReadBuffer.GetUseSize())
		{
			reinterpret_cast<Util::OutputMemoryStream&>(Stream).Serialize(Buffer[1].BufferPtr, Buffer[1].length);
		}

		return Stream;
	}

	bool IDescriptor::IsVaildCallback() const
	{
		return !(_ReadCallback == nullptr ||
			_WriteCallback != nullptr ||
			_ExceptCallback != nullptr ||
			_Key != nullptr);
	}

	Util::CircularBuffer IDescriptor::GetWriteBuffer() const
	{
		return _WriteBuffer;
	}


}