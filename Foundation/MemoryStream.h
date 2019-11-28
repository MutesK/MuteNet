#pragma once


namespace Util
{
	using Byte = char;

	class MemoryStream
	{
	protected:
		static const size_t BUFFER_LENGTH = 20000;

		Byte* _buffer;

		__int64 _head = 0;
		__int64 _tail = 0;
		__int64 _capacity = BUFFER_LENGTH;
	public:
		MemoryStream();
		virtual ~MemoryStream();

		virtual void Serialize(void* Data, uint32_t ByteCount) = 0;
		const char* GetBufferPtr() const;
	};


	inline const char* MemoryStream::GetBufferPtr() const
	{
		return _buffer;
	}
}
