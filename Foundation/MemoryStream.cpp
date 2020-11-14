#include "FoundationCommon.h"
#include "MemoryStream.h"

namespace Util
{
	MemoryStream::MemoryStream ( )
	{
		_Buffer = new Byte[_Capacity];
	}
	
	MemoryStream::~MemoryStream ( )
	{
		delete[] _Buffer;
	}
}
