#pragma once

namespace Util
{
	class HeapBlock : public std::enable_shared_from_this<HeapBlock>
	{
	private:
		char* _buffer;

		__int64 _head = 0;
		__int64 _tail = 0;
		__int64 _capacity = 0;

		static TL::ObjectPool<HeapBlock> BlockPool;
	public:
		static std::shared_ptr<HeapBlock> make_shared();

		HeapBlock() = default;
		~HeapBlock();

		void Write(const void* inData, const uint32_t bytesize);
		void Read(void* outData, const uint32_t bytesize);

		char* GetBufferPtr();

		void MoveWritePosition(const uint32_t length);

		NON_COPYABLE(HeapBlock);

		friend class InputMemoryStream;
		friend class OutputMemoryStream;

		void ReallocBuffer(int buffer);
	};

}
