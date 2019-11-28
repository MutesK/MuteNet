#pragma once

#include "MemoryStream.h"

namespace Util
{
	class OutputMemoryStream : public MemoryStream
	{
	public:
		OutputMemoryStream() = delete;
		virtual ~OutputMemoryStream() = default;

		virtual __int64 GetLength() const;

		virtual void Write(const void* inData, const uint32_t inByteCount);

		template <typename Type>
		void Write(const Type& inData);

		template <typename Type>
		void Write(const std::vector<Type>& vector);

		template <>
		void Write(const std::string& inData);

		virtual void Serialize(void* inData, uint32_t inByteCount) override;

		void MoveWritePosition(__int64 size);
	};

	inline __int64 OutputMemoryStream::GetLength() const
	{
		return _tail;
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
		unsigned short length = static_cast<unsigned short>(inData.length());

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

	inline void OutputMemoryStream::MoveWritePosition(__int64 size)
	{
		_tail += size;
	}

}