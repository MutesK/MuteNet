#include "CircularTest.h"
#include <WinSock2.h>
#include "../Foundation/OutputMemoryStream.h"
#include "../Foundation/InputMemoryStream.h"

const std::string inputData =
"HELLO_UNITTEST000000000000000000000000000000000000000000000HELLO_UNITTEST000000000000000000000000000000000000000000000HELLO_UNITTEST000000000000000000000000000000000000000000000HELLO_UNITTEST000000000000000000000000000000000000000000000";

char buffer[25000]{ '\0' };
uint32_t inputDataSize = inputData.length();

namespace Util
{
	CircularTest::CircularTest()
	{
		_Buffer = CircularBuffer::Alloc();
	}

	void CircularTest::SendPacket()
	{
		auto WriteStream = std::make_shared<OutputMemoryStream>();

		WriteStream->Serialize((void *)inputData.c_str(), inputDataSize);
		
		_Buffer->PutData(WriteStream->GetBufferPtr(), WriteStream->GetLength());
	}

	void CircularTest::RecvProcess()
	{
		uint32_t length = 0;

		while(true)
		{
			length = 0;
			_Buffer->Peek(&length, sizeof(uint32_t));

			if (_Buffer->GetUsedSize() < (length + sizeof(uint32_t)))
				break;

			const auto packetSize = length + sizeof(uint32_t);
			auto HeapBlock = HeapBlock::make_shared();

			_Buffer->GetData(HeapBlock->GetBufferPtr(), packetSize);

			auto InputStream = std::make_shared<InputMemoryStream>(HeapBlock);
			DataChecker(InputStream);
		}
	}

	void CircularTest::DataChecker(std::shared_ptr<InputMemoryStream>& Stream) const
	{
		memset(buffer, 0, sizeof(25000));
		Stream->Serialize(buffer, inputDataSize);

		if (strncmp(buffer, inputData.c_str(), inputDataSize) != 0)
			throw;
	}
}
