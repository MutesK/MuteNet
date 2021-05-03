#include "Common.h"
#include "OutputMemoryStream.h"
#include "InputMemoryStream.h"

using namespace Util;


void OutputMemoryStream::Write(const void* inData, const uint32_t inByteCount)
{
	const uint64_t resultPosition = _Tail + inByteCount;

	if (resultPosition > static_cast<const uint64_t>(_Capacity))
	{
		throw;
	}

	std::memcpy(_Buffer + _Tail, inData, inByteCount);
	_Tail += inByteCount;
}


void OutputMemoryStream::Serialize(void* inData, uint32_t inByteCount)
{
	Write(&inByteCount, sizeof(uint32_t));
	Write(inData, inByteCount);
}
