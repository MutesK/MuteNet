#pragma once

#include "MemoryStream.h"

namespace Util
{
	class OutputMemoryStream : public MemoryStream
	{
	protected:
		char* _buffer = nullptr;

		uint32_t _head = 0;
		uint32_t _capacity = 0;
	public:
		OutputMemoryStream();
		virtual ~OutputMemoryStream();

		char* GetBufferPtr();
		virtual uint32_t GetLength() const;

		virtual void Write(const void* inData, uint32_t inByteCount);

		template <typename Type>
		void Write(const Type& inData);

		template <typename Type>
		void Write(const std::vector<Type>& vector);

		virtual void Serialize(void* inData, uint32_t inByteCount) override;
		virtual bool IsInput() override;

		void MoveWritePosition(uint32_t size);
		void ReallocBuffer(uint32_t inNewLength);
	};


	inline char* OutputMemoryStream::GetBufferPtr()
	{
		return _buffer;
	}

	inline uint32_t OutputMemoryStream::GetLength() const
	{
		return _head;
	}

	template <typename Type>
	void OutputMemoryStream::Write(const Type& inData)
	{
		unsigned short length = sizeof(unsigned short);

		Write(&length, sizeof(size_t));
		Write(&inData, sizeof(unsigned short));
	}

	template <typename Type>
	void OutputMemoryStream::Write(const std::vector<Type>& vector)
	{
		unsigned short length = static_cast<unsigned short>(vector.size());

		Write(&length, sizeof(unsigned short));

		for (const auto& elem : vector)
		{
			Write(elem);
		}

	}

	inline bool OutputMemoryStream::IsInput()
	{
		return false;
	}

	inline void OutputMemoryStream::MoveWritePosition(uint32_t size)
	{
		_head += size;
	}

}