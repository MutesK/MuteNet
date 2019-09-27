#pragma once

#include "MemoryStream.h"
#include "HeapBlock.h"

namespace Util
{
	class OutputMemoryStream : public MemoryStream
	{
	protected:
		std::shared_ptr<HeapBlock> _heapBlock;
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

		template <>
		void Write(const std::string& inData);

		virtual void Serialize(void* inData, uint32_t inByteCount) override;
		virtual bool IsInput() override;

		void MoveWritePosition(uint32_t size);

		std::shared_ptr<HeapBlock> GetHeapBlock();
	};


	inline char* OutputMemoryStream::GetBufferPtr()
	{
		return _heapBlock->_buffer;
	}

	inline uint32_t OutputMemoryStream::GetLength() const
	{
		return _heapBlock->_tail;
	}

	template <typename Type>
	void OutputMemoryStream::Write(const Type& inData)
	{
		unsigned short length = sizeof(unsigned short);

		Write(&length, sizeof(unsigned short));
		Write(&inData, length);
	}

	template <>
	void OutputMemoryStream::Write(const std::string& inData)
	{
		unsigned short length = inData.length();

		Write(&length, sizeof(unsigned short));
		Write(inData.c_str(), length);
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
		_heapBlock->_tail += size;
	}

	inline std::shared_ptr<HeapBlock> OutputMemoryStream::GetHeapBlock()
	{
		return _heapBlock;
	}

}