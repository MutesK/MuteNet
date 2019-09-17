#pragma once

#include "MemoryStream.h"

namespace Util
{
	class InputMemoryStream : public MemoryStream
	{
	protected:
		char* _buffer = nullptr;

		uint32_t _head = 0;
		uint32_t _capacity = 0;
	public:
		InputMemoryStream(char* buffer, uint32_t inByteCount);
		virtual ~InputMemoryStream();

		virtual uint32_t GetRemainingDataSize() const;

		virtual void Read(void* outData, uint32_t inByteCount);

		template <typename Type>
		void Read(Type& outData);

		template <typename Type>
		void Read(std::vector<Type>& vector);

		virtual bool IsInput() override;
		virtual void Serialize(void* outData, uint32_t inByteCount) override;
	};

	inline uint32_t InputMemoryStream::GetRemainingDataSize() const
	{
		return _capacity - _head;
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