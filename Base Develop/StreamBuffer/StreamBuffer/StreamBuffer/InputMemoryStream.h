#pragma once
/*
	이미 담긴 데이터를 Byte에 맞게 가져옴
*/
class OutputMemoryStream;
class InputMemoryStream : public MemoryStream
{
public:
	InputMemoryStream(std::shared_ptr<char>& buffer, uint32_t inByteCount);
	InputMemoryStream(OutputMemoryStream& outputStream);

	~InputMemoryStream();

	std::shared_ptr<char>&  GetBufferPtr();

	void   MoveRecvSize(size_t RecvSize);
	uint32_t GetRemainingDataSize() const;


	void Read(void *pOutData, uint32_t ByteCount);
	
	template <typename type>
	void Read(type& pOutData)
	{
		Read(&pOutData, sizeof(pOutData));
	}

	template <typename type>
	InputMemoryStream& operator >> (type& pOutData)
	{
		Read(&pOutData, sizeof(pOutData));
		return *this;
	}
	virtual void Serialize(void *ioData, uint32_t Bytecount) override;
	virtual bool IsInput() const override;
private:
	std::shared_ptr<char> _buffer;
	uint32_t _head;
	uint32_t _capacity;

	const uint32_t _MaxPacketSize = 1470;
};

inline void   InputMemoryStream::MoveRecvSize(size_t RecvSize)
{
	if (_head + RecvSize >= _capacity)
	{
		*((int *)0) = 1; // Crash
	}

	_head += RecvSize;
}

inline uint32_t InputMemoryStream::GetRemainingDataSize() const
{
	return _capacity - _head;
}

inline std::shared_ptr<char>&  InputMemoryStream::GetBufferPtr()
{
	return _buffer;
}


