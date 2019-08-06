#pragma once

#include "MemoryStream.h"
#include "InputMemoryStream.h"
#include "InputByteStream.h"
#include "OutputMemoryStream.h"
#include "OutputByteStream.h"

namespace Util
{
	namespace Pool
	{
		// Ring Buffer Pool
		static TL::ObjectPool<OutputByteStream> ByteBufferPool;
		// Packet Unit Buffer Pool
		static TL::ObjectPool<OutputMemoryStream> PacketBufferPool;
	}
}