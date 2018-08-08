#pragma once

/*
	출력 스트림

	순서대로 데이터 원소를 넣을수 있지만, 읽어 들이지는 못한다.
	주로 송신용 버퍼스트림으로 사용할예정이다.
*/
class OutputMemoryStream : public MemoryStream
{
public:
	OutputMemoryStream();
	~OutputMemoryStream();

	std::shared_ptr<char>& GetBufferPtr();
	uint32_t GetLength() const;
	
	void Write(const void* inData, size_t inByteCount);
	
	template<typename type>
	void Write(const type& inData)
	{
		// 원시 자료형 검사
		static_assert(std::is_arithmetic<type>::value || std::is_enum<type>::value, "Generic Write Only Supports Primitive Data Type");

		Write(&inData, sizeof(inData));
	}

	template<typename type>
	OutputMemoryStream& operator<<(const type& inData)
	{
		Write(inData);
		return *this;
	}

	virtual void Serialize(void *ioData, uint32_t Bytecount) override;
	virtual bool IsInput() const override;
private:
	void ReallocBuffer(uint32_t inNewLength);
private:
	std::shared_ptr<char> _buffer;
	uint32_t _head;
	uint32_t _capacity; 
};

