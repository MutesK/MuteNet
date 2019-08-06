#pragma once

#include "OutputMemoryStream.h"

namespace Util
{
	class OutputByteStream final : public OutputMemoryStream
	{
	protected:
		uint32_t _tail = 0;
	public:
		OutputByteStream();
		virtual ~OutputByteStream() = default;

		uint32_t GetLength() const override;

		void Write(const void* inData, uint32_t inByteCount) override;

		int GetFreeSize() const;
	};
}

