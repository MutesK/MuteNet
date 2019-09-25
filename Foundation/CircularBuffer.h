#pragma once

namespace Util
{
	class CircularBuffer final 
	{
	public:
		using Ptr = decltype(TL::UniqueCustomDeleter<CircularBuffer>::DeletorType);
		std::recursive_mutex _mutex;
	private:
		const size_t BUFFER_SIZE = 25000;

		char* _buffer = nullptr;

		size_t _head = 0;
		size_t _tail = 0;
	public:
		CircularBuffer();
		~CircularBuffer();

		static auto Alloc() ->Ptr;

		void Reset();
		bool isEmpty() const;
		size_t GetFreeSize() const;
		size_t GetUsedSize() const;

		uint32_t GetWriteBufferAndLengths(void* firstBuffer, size_t& firstLength,
			void* secondBuffer, size_t& secondLength);
		uint32_t GetReadBufferAndLengths(void* firstBuffer, size_t& firstLength,
			void* secondBuffer, size_t& secondLength);

		void MoveReadPostion(const size_t& position);
		size_t Peek(void* outData, const size_t size);
		size_t GetData(void* outData, const size_t size);
		
		void MoveWritePostion(const size_t& position);
		size_t PutData(void* inData, const size_t size);

		char* GetWriteBufferPtr();
		char* GetReadBufferPtr();

		NON_COPYABLE(CircularBuffer);
	};

	inline void CircularBuffer::Reset()
	{
		_head = _tail = 0;
	}
	inline bool CircularBuffer::isEmpty() const
	{
		return GetUsedSize() == 0;
	}
	inline size_t CircularBuffer::GetFreeSize() const
	{
		return BUFFER_SIZE - GetUsedSize();
	}
	inline size_t CircularBuffer::GetUsedSize() const
	{
		if (_tail >= _head)
			return _tail - _head;

		return BUFFER_SIZE - _head + _tail;
	}

	inline char* CircularBuffer::GetWriteBufferPtr()
	{
		return _buffer + _tail;
	}

	inline char* CircularBuffer::GetReadBufferPtr()
	{
		return _buffer + _head;
	}

}