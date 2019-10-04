#include "pch.h"
#include "OutputMemoryStream.h"
#include "InputMemoryStream.h"
#include "HeapBlock.h"

using namespace Util;

OutputMemoryStream::OutputMemoryStream()
{
	_heapBlock = HeapBlock::Alloc();
}

OutputMemoryStream::~OutputMemoryStream()
{
	
}

void OutputMemoryStream::Write(const void* inData, const uint32_t inByteCount)
{
	_mutex.lock();
	_heapBlock->Write(inData, inByteCount);
	_mutex.unlock();
}


void OutputMemoryStream::Serialize(void* inData, uint32_t inByteCount)
{
	Write(&inByteCount, sizeof(uint32_t));
	Write(inData, inByteCount);
}
