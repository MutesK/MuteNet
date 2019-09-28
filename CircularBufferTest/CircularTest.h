#pragma once

#include "../Foundation/pch.h"
#include "../Foundation/CircularBuffer.h"

namespace Util
{
	class InputMemoryStream;
	class CircularTest
	{
	private:
		CircularBuffer::Ptr _Buffer;
	public:
		CircularTest();

		void SendPacket();
		void RecvProcess();
		void DataChecker(std::shared_ptr<InputMemoryStream>& Stream) const;
	};
}

