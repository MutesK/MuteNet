#pragma once

#include "MemoryStream.h"
#include "HeapBlock.h"

namespace Util
{
	class InputMemoryStream : public MemoryStream
	{
	protected:
		std::shared_ptr<Util::HeapBlock> _heapBlock;
	public:
		InputMemoryStream(std::shared_ptr<Util::HeapBlock>& _heapBlock);
		virtual ~InputMemoryStream() = default;

		virtual __int64 GetRemainingDataSize() const;

		virtual void Read(void* outData, uint32_t inByteCount);

		template <typename Type>
		void Read(Type& outData);

		template <typename Type>
		void Read(std::vector<Type>& vector);

		virtual bool IsInput() override;
		virtual void Serialize(void* outData, uint32_t inByteCount) override;
	};

	inline __int64 InputMemoryStream::GetRemainingDataSize() const
	{
		return _heapBlock->_head - _heapBlock->_tail;
	}

	template <typename Type>
	void InputMemoryStream::Read(Type& outData)
	{
		Read(&outData, sizeof(Type));
	}

	template <typename Type>
	void InputMemoryStream::Read(std::vector<Type>& vector)
	{
		size_t elemCount = 0;
		Read(elemCount);
		vector.resize(elemCount);

		for (Type& elem : vector)
		{
			Read(elem);
		}
	}

	inline bool InputMemoryStream::IsInput()
	{
		return true;
	}

}