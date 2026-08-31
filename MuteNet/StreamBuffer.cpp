#include "Common.h"
#include "StreamBuffer.h"
#include "Allocator.h" // 정돈된 Allocator 헤더 포함

namespace MuteNet
{
	StreamBuffer::StreamBuffer(size_t initialCapacity)
	{
		initialCapacity = std::max<size_t>(1, initialCapacity);
		maxCapacity = initialCapacity;

		auto& res = Allocator::GetResource();
		buffer = static_cast<Byte*>(res.allocate(maxCapacity, alignof(Byte)));

		head = 0;
		tail = 0;
		currentSize = 0;
	}

	StreamBuffer::~StreamBuffer()
	{
		if (buffer)
		{
			auto& res = Allocator::GetResource();
			res.deallocate(buffer, maxCapacity, alignof(Byte));
			buffer = nullptr;
		}
	}

	void StreamBuffer::reserve(const size_t length)
	{
		if (maxCapacity - currentSize >= length) return;

		size_t oldCapacity = maxCapacity;
		// 필요한 크기를 만족할 때까지 2배씩 확장
		while (maxCapacity - currentSize < length) {
			maxCapacity *= 2;
		}

		auto& res = Allocator::GetResource();

		Byte* newBuffer = static_cast<Byte*>(res.allocate(maxCapacity, alignof(Byte)));

		if (currentSize > 0) {
			if (head < tail) {
				std::memcpy(newBuffer, buffer + head, currentSize);
			}
			else {
				size_t rightSize = oldCapacity - head;
				std::memcpy(newBuffer, buffer + head, rightSize);
				std::memcpy(newBuffer + rightSize, buffer, tail);
			}
		}

		res.deallocate(buffer, oldCapacity, alignof(Byte));

		buffer = newBuffer;
		head = 0;
		tail = currentSize;
	}

	void StreamBuffer::Write(const BytePtr data, const size_t length)
	{
		if (maxCapacity - currentSize < length)
		{
			reserve(length);
		}

		if (tail >= head)
		{
			size_t right = maxCapacity - tail;
			if (right >= length)
			{
				std::memcpy(buffer + tail, data, length);
			}
			else
			{
				std::memcpy(buffer + tail, data, right);
				std::memcpy(buffer, (const Byte*)data + right, length - right);
			}
		}
		else
		{
			std::memcpy(buffer + tail, data, length);
		}

		tail = (tail + length) % maxCapacity;
		currentSize += length;
	}

	bool StreamBuffer::Read(BytePtr data, const size_t length)
	{
		if (length > currentSize) return false;

		if (data) {
			Peek(data, length);
		}

		head = (head + length) % maxCapacity;
		currentSize -= length;

		if (currentSize == 0) {
			head = 0;
			tail = 0;
		}
		return true;
	}

	bool StreamBuffer::Peek(BytePtr data, const size_t length)
	{
		if (length > currentSize) return false;

		if (head + length <= maxCapacity) {
			std::memcpy(data, buffer + head, length);
		}
		else {
			size_t rightSize = maxCapacity - head;
			std::memcpy(data, buffer + head, rightSize);
			std::memcpy(data + rightSize, buffer, length - rightSize);
		}
		return true;
	}
}