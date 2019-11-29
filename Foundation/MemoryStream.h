#pragma once


namespace Util
{
	using Byte = char;

	class MemoryStream
	{
	protected:
		Byte* _Buffer;

		__int64 _Head = 0;
		__int64 _Tail = 0;
		__int64 _Capacity = BUFFER_LENGTH;
	public:
		MemoryStream();
		virtual ~MemoryStream();

		virtual void Serialize(void* Data, uint32_t ByteCount) = 0;
		const char* GetBufferPtr() const;
	};


	inline const char* MemoryStream::GetBufferPtr() const
	{
		return _Buffer;
	}
}
