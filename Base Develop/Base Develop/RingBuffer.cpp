#include <iostream>
#include "RingBuffer.h"

RingBuffer::RingBuffer()
	:m_front(0), m_rear(0), m_bufferSize(enBUFFER_SIZE)
{
	m_buffer = new char[enBUFFER_SIZE];
	memset(m_buffer, 0, enBUFFER_SIZE);
}

RingBuffer::RingBuffer(int iSize)
	:m_bufferSize(iSize), m_front(0), m_rear(0)
{
	m_buffer = new char[iSize];
	memset(m_buffer, 0, iSize);
}


RingBuffer::~RingBuffer()
{
	delete[] m_buffer;
}

int	RingBuffer::GetBufferSize(void)
{
	return m_bufferSize - enBUFFER_BLANK;
}

/////////////////////////////////////////////////////////////////////////
// ���� ������� �뷮 ���.
//
// Parameters: ����.
// Return: (int)������� �뷮.
/////////////////////////////////////////////////////////////////////////
int	RingBuffer::GetUseSize(void)
{
	if (m_rear >= m_front)
		return m_rear - m_front;
	else
		return m_bufferSize - m_front + m_rear;
}

/////////////////////////////////////////////////////////////////////////
// ���� ���ۿ� ���� �뷮 ���.
//
// Parameters: ����.
// Return: (int)�����뷮.
/////////////////////////////////////////////////////////////////////////
int	RingBuffer::GetFreeSize(void)
{
	return m_bufferSize - GetUseSize() - enBUFFER_BLANK;
}

/////////////////////////////////////////////////////////////////////////
// ���� �����ͷ� �ܺο��� �ѹ濡 �а�, �� �� �ִ� ����.
// (������ ���� ����)
//
// Parameters: ����.
// Return: (int)��밡�� �뷮.
////////////////////////////////////////////////////////////////////////
int	RingBuffer::GetNotBrokenGetSize(void)
{
	if (m_front <= m_rear)
	{
		return m_rear - m_front;
	}
	else
		return m_bufferSize - m_front;
}
int	RingBuffer::GetNotBrokenPutSize(void)
{
	if (m_rear < m_front)
	{
		return m_front - m_rear - enBUFFER_BLANK;
	}
	else
	{
		if (m_front < enBUFFER_BLANK)
			return m_bufferSize - m_rear - enBUFFER_BLANK + m_front;
		else
			return m_bufferSize - m_rear;
	}
}

/////////////////////////////////////////////////////////////////////////
// WritePos �� ����Ÿ ����.
//
// Parameters: (char *)����Ÿ ������. (int)ũ��. 
// Return: (int)���� ũ��.
/////////////////////////////////////////////////////////////////////////
int	RingBuffer::PutData(char *chpData, int iSize)
{
	int inputSize = 0;

	if (iSize > GetFreeSize())
		return 0;

	if (iSize <= 0)
		return 0;

	if (m_front <= m_rear)
	{
		int WriteSize = m_bufferSize - m_rear;

		if (WriteSize >= iSize)
		{
			memcpy(m_buffer + m_rear, chpData, iSize);
			m_rear += iSize;
		}
		else
		{
			memcpy(m_buffer + m_rear, chpData, WriteSize);
			memcpy(m_buffer, chpData + WriteSize, iSize - WriteSize);
			m_rear = iSize - WriteSize;
		}

	}
	else
	{
		memcpy(m_buffer + m_rear, chpData, iSize);
		m_rear += iSize;
	}
}

/////////////////////////////////////////////////////////////////////////
// ReadPos ���� ����Ÿ ������. ReadPos �̵�.
//
// Parameters: (char *)����Ÿ ������. (int)ũ��.
// Return: (int)������ ũ��.
/////////////////////////////////////////////////////////////////////////
int	RingBuffer::GetData(char *chpDest, int iSize)
{
	if (iSize > GetUseSize())
		return 0;

	if (iSize <= 0)
		return 0;

	if (m_front <= m_rear)
	{
		memcpy(chpDest, m_buffer + m_front, iSize);
		m_front += iSize;
	}
	else
	{
		int ReadSize = m_bufferSize - m_front;

		if (ReadSize >= iSize)
		{
			memcpy(chpDest, m_buffer + m_front, iSize);
			m_front += iSize;
		}
		else
		{
			memcpy(chpDest, m_buffer + m_front, ReadSize);
			memcpy(chpDest + ReadSize, m_buffer, iSize - ReadSize);
			m_front = iSize - ReadSize;
		}
	}

	return iSize;
}

int	RingBuffer::Peek(char *chpDest, int iSize)
{
	if (iSize > GetUseSize())
		return 0;

	if (iSize <= 0)
		return 0;

	if (m_front <= m_rear)
	{
		memcpy(chpDest, m_buffer + m_front, iSize);
	}
	else
	{
		int ReadSize = m_bufferSize - m_front;

		if (ReadSize >= iSize)
		{
			memcpy(chpDest, m_buffer + m_front, iSize);
		}
		else
		{
			memcpy(chpDest, m_buffer + m_front, ReadSize);
			memcpy(chpDest + ReadSize, m_buffer, iSize - ReadSize);
		}
	}

	return iSize;
}

void RingBuffer::RemoveData(int iSize)
{
	if (iSize > GetUseSize())
		return;

	if (iSize <= 0)
		return;

	if (m_front <= m_rear)
	{
		m_front += iSize;
	}
	else
	{
		int ReadSize = m_bufferSize - m_front;

		if (ReadSize >= iSize)
		{
			m_front += iSize;
		}
		else
		{
			m_front = iSize - ReadSize;
		}
	}
}

void RingBuffer::MoveWritePos(int iSize)
{
	int inputSize = 0;

	if (iSize > GetFreeSize())
		return;

	if (iSize <= 0)
		return;

	if (m_front <= m_rear)
	{
		int WriteSize = m_bufferSize - m_rear;

		if (WriteSize >= iSize)
		{
			m_rear += iSize;
		}
		else
		{
			m_rear = iSize - WriteSize;
		}

	}
	else
	{
		m_rear += iSize;
	}
}

void RingBuffer::ClearBuffer(void)
{
	m_front = 0;
	m_rear = 0;
}

char* RingBuffer::GetBufferPtr(void)
{
	return m_buffer;
}

char* RingBuffer::GetReadBufferPtr(void)
{
	return m_buffer + m_front;
}


char* RingBuffer::GetWriteBufferPtr(void)
{
	return m_buffer + m_rear;
}

RingBuffer	&RingBuffer::operator = (RingBuffer &clSrStreamBuffer)
{
	if (m_buffer != nullptr)
	{
		delete[] m_buffer;
		m_buffer = nullptr;
	}

	m_bufferSize = clSrStreamBuffer.m_bufferSize;

	m_front = clSrStreamBuffer.m_front;
	m_rear = clSrStreamBuffer.m_rear;

	m_buffer = new char[m_bufferSize];

	memcpy(m_buffer, clSrStreamBuffer.m_buffer, sizeof(m_bufferSize));

	return *this;
}