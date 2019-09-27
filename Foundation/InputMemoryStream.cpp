#include "pch.h"
#include "InputMemoryStream.h"
using namespace Util;

InputMemoryStream::InputMemoryStream(std::shared_ptr<Util::HeapBlock>& heapBlock)
	:_heapBlock(heapBlock)
{
}

InputMemoryStream::~InputMemoryStream()
{
}

// Heap Corruption À¯ÀÇ
void InputMemoryStream::Read(void* outData, uint32_t inByteCount)
{
	_mutex.lock();
	_heapBlock->Read(outData, inByteCount);
	_mutex.unlock();
}


void InputMemoryStream::Serialize(void* outData, uint32_t inByteCount)
{
	uint32_t Size = 0;

	Read(&Size, sizeof(uint32_t));

	if (Size > inByteCount)
	{
		outData = nullptr;
		return;
	}

	Read(outData, Size);
}




