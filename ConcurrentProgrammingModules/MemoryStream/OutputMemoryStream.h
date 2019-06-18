#pragma once

#include "MemoryStream.h"

class OutputMemoryStream final : public MemoryStream
{
private:
	char* _buffer;

	uint32_t _head;
	uint32_t _capacity;
public:
	OutputMemoryStream();
	virtual ~OutputMemoryStream();

	const char* GetBufferPtr() const;
	uint32_t GetLength() const;

	void Write(const void* inData, uint32_t inByteCount);

	template <typename Type>
	void Write(const Type& inData);

	template <typename Type>
	void Write(const std::vector<Type>& vector);

	virtual void Serialize(void* inData, uint32_t inByteCount) override;
	virtual bool IsInput() override;

private:
	void ReallocBuffer(uint32_t inNewLength);
};


inline const char* OutputMemoryStream::GetBufferPtr() const
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
	static_assert(std::is_arithmetic_v<Type> ||
		std::is_enum_v<Type>,
		"Generic Write Only Supports Primitive Data Types");

	Write(&inData, sizeof(Type));
}

template <typename Type>
void OutputMemoryStream::Write(const std::vector<Type>& vector)
{
	size_t length = vector.size();

	Write(&length, sizeof(size_t));

	for (const auto& elem : vector)
	{
		Write(elem);
	}

}

inline bool OutputMemoryStream::IsInput()
{
	return false;
}

