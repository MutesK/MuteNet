#pragma once

namespace Util
{
	class HeapBlock : public std::enable_shared_from_this<HeapBlock>
	{
	private:
		char* _buffer;

		uint32_t _head = 0;
		uint32_t _tail = 0;
		uint32_t _capacity = 0;

		static TL::ObjectPool<HeapBlock> BlockPool;
	public:
		static std::shared_ptr<HeapBlock> make_shared();

		HeapBlock() = default;
		~HeapBlock();

		void Write(const void* inData, const uint32_t bytesize);
		void Read(void* outData, const uint32_t bytesize);

		NON_COPYABLE(HeapBlock);

		friend class InputMemoryStream;
		friend class OutputMemoryStream;
	private:
		void ReallocBuffer(int buffer);
	};

}
