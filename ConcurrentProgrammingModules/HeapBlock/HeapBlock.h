#pragma once
#include <cstdint>

/*
 * RingBuffer
 *  - Realloc 되지 않는다.
 *  Payload
 *  [StreamBuffer ByteSize][StreamBuffer Payload]
 *  즉 실제로 보내지는건
 */

class HeapBlock 
{
private:
	char* _buffer = nullptr;
	uint32_t _front = 0;
	uint32_t _rear = 0;
	uint32_t _capacity = 0;
public:
	HeapBlock();
	~HeapBlock();

	uint32_t GetCapacitySize();
	uint32_t GetUseSize();
	uint32_t GetFreeSize();

	uint32_t GetHeapInputSize();
	uint32_t GetHeapOutputSize();

	uint32_t Peek(char* Data, uint32_t Size);

	void Clear();

	const void* GetBufferPtr();
	const void* GetInputBufferPtr();
	const void* GetOutputBufferPtr();

	void MoveInputPosition(uint32_t Size);
	void MoveOutputPosition(uint32_t Size);

	uint32_t PutData(const char* Data, uint32_t Size);
	uint32_t GetData(char* Data, uint32_t Size);
};

inline uint32_t HeapBlock::GetCapacitySize()
{
	return _capacity;
}


inline uint32_t HeapBlock::GetUseSize()
{
	if (_rear >= _front)
		return _rear - _front;

	return _capacity - _front + _rear;
}

inline uint32_t HeapBlock::GetFreeSize()
{
	return _capacity - GetUseSize();
}

inline uint32_t HeapBlock::GetHeapInputSize()
{
	if (_rear < _front)
		return _front - _rear;
	else
		return _capacity - _rear;
}

inline uint32_t HeapBlock::GetHeapOutputSize()
{
	if (_front <= _rear)
		return _rear - _front;

	return _capacity - _front;
}

inline void HeapBlock::Clear()
{
	_front = _rear = 0;
}

inline const void* HeapBlock::GetBufferPtr()
{
	return _buffer;
}

inline const void* HeapBlock::GetInputBufferPtr()
{
	return _buffer + _rear;
}

inline const void* HeapBlock::GetOutputBufferPtr()
{
	return _buffer + _front;
}






