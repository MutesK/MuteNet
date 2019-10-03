#pragma once

namespace Util
{
	class HeapBlock final
	{
	private:
		static const size_t BUFFER_LENGTH = 20000;

		char* _buffer;

		__int64 _head = 0;
		__int64 _tail = 0;
		__int64 _capacity = BUFFER_LENGTH;

		static TL::ObjectPool<HeapBlock> BlockPool;
	public:
		static std::shared_ptr<HeapBlock> make_shared();

		HeapBlock();
		~HeapBlock();

		void Write(const void* inData, const size_t bytesize);
		void Read(void* outData, const size_t bytesize);

		char* GetBufferPtr();

		void MoveWritePosition(const size_t length);

		NON_COPYABLE(HeapBlock);

		friend class InputMemoryStream;
		friend class OutputMemoryStream;
	};

}