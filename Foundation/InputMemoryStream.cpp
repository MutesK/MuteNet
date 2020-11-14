#include "FoundationCommon.h"
#include "InputMemoryStream.h"

using namespace Util;

// Heap Corruption ����
void InputMemoryStream::Read ( void *outData, uint32_t inByteCount )
{
	if ( outData == nullptr )
	{
		throw;
	}
	
	if ( static_cast<uint64_t>(_Head) + inByteCount > static_cast<const uint64_t>(_Capacity))
	{
		throw;
	}
	
	memcpy ( outData, _Buffer + _Head, inByteCount );
	_Head += inByteCount;
}

void InputMemoryStream::Serialize ( void *outData, uint32_t inByteCount )
{
	uint32_t Size = 0;
	
	Read ( &Size, sizeof ( uint32_t ));
	
	if ( Size > inByteCount )
	{
		outData = nullptr;
		return;
	}
	
	Read ( outData, Size );
}




