#pragma once

#include "../Foundation/pch.h"

namespace Util
{
	class OutputMemoryStream;
	class StreamTest
	{
	private:
		std::shared_ptr<OutputMemoryStream> Stream;
	public:
		StreamTest();

		void EnqueueDataStream();
		void TestGetBufferElements();
	};
}
