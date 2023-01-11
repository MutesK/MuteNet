#ifndef MUTENET_OUTPUTMEMORYSTREAM_HPP
#define MUTENET_OUTPUTMEMORYSTREAM_HPP

#include "MemoryStream.h"

namespace Util
{
	class OutputMemoryStream : public MemoryStream
	{
	public:
		OutputMemoryStream() = default;

		virtual ~OutputMemoryStream() = default;

		virtual int64_t GetLength() const;

		virtual void Write(const void* inData, const uint32_t inByteCount);

		template<typename Type>
		void Write(const Type& inData)
		{
			unsigned short length = sizeof(unsigned short);

			Write(&length, sizeof(unsigned short));
			Write(&inData, length);
		}


		template<typename Type>
		void Write(const std::vector<Type>& vector)
		{
			unsigned short length = static_cast<unsigned short>(vector.size());

			Write(&length, sizeof(unsigned short));

			for (const auto& elem : vector)
			{
				Write(elem);
			}

		}

		template<>
		void Write(const std::string& inData)
		{
			unsigned short length = static_cast<unsigned short>(inData.length());

			Write(&length, sizeof(unsigned short));
			Write(inData.c_str(), length);
		}

		virtual void Serialize(void* inData, uint32_t inByteCount) override;

		void MoveWritePosition(int64_t size);

		void Clear();
	};

}

#endif