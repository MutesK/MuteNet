#pragma once

/*
	��� ��Ʈ��

	������� ������ ���Ҹ� ������ ������, �о� �������� ���Ѵ�.
	�ַ� �۽ſ� ���۽�Ʈ������ ����ҿ����̴�.
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
		// ���� �ڷ��� �˻�
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

