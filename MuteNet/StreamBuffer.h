#pragma once

namespace MuteNet
{
	using Byte = unsigned char;
	using BytePtr = Byte*;

	class StreamBuffer final
	{
		BytePtr buffer;
		size_t maxCapacity;
		size_t head;
		size_t tail;
		size_t currentSize;

		void reserve(const size_t length);
	public:
		explicit StreamBuffer(size_t initialCapacity = 1024);
		~StreamBuffer();
		
		void Write(const BytePtr data, const size_t length);
		bool Read(BytePtr data, const size_t length);
		bool Peek(BytePtr data, const size_t length);

		size_t GetSize() const { return currentSize; }
		size_t GetCapacity() const { return maxCapacity; }
	};
}