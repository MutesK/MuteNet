#pragma once

namespace Util
{
	class CircularBuffer final 
	{
	private:
		const size_t BUFFER_SIZE = 25000;

		char* _buffer = nullptr;

		size_t _head = 0;
		size_t _tail = 0;

		std::recursive_mutex _mutex;
	public:
		CircularBuffer();
		~CircularBuffer();

		static std::unique_ptr<CircularBuffer> Alloc();

		void Reset();
		bool isEmpty() const;
		size_t GetFreeSize() const;
		size_t GetUsedSize() const;

		uint32_t GetWrtieBufferPtr(void* firstBuffer, size_t& firstLength,
			void* secondBuffer, size_t& secondLength);

		void MoveReadPostion(const size_t& position);
		size_t Peek(void* outData, const size_t size);
		size_t GetData(void* outData, const size_t size);
		
		void MoveWritePostion(const size_t& position);
		size_t PutData(void* inData, const size_t size);

		NON_COPYABLE(CircularBuffer);
	private:
		void Lock();
		void UnLock();
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
}