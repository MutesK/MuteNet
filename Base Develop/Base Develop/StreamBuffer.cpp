
#include "PacketBuffer.h"

StreamBuffer::StreamBuffer()
{
	m_chpBuffer = new char[eBUFFER_DEFAULT];
	ZeroMemory(m_chpBuffer, eBUFFER_DEFAULT);
	m_iDataSize = 0;
	m_iBufferSize = eBUFFER_DEFAULT;

	m_chpReadPos = m_chpBuffer;
	m_chpWritePos = m_chpBuffer;
}

StreamBuffer::StreamBuffer(int iBufferSize)
{
	m_chpBuffer = new char[iBufferSize];
	ZeroMemory(m_chpBuffer, iBufferSize);
	m_iDataSize = 0;
	m_iBufferSize = iBufferSize;


	m_chpReadPos = m_chpBuffer;
	m_chpWritePos = m_chpBuffer;
}

StreamBuffer::~StreamBuffer()
{
	Release();
}


void StreamBuffer::Release(void)
{
	ClearBuffer();
}
void	StreamBuffer::ClearBuffer(void)
{
	m_iDataSize = 0;

	m_chpReadPos = m_chpBuffer;
	m_chpWritePos = m_chpBuffer;
	ErrorCode = 0;
}

int	StreamBuffer::MoveWritePos(int iSize)
{
	if (iSize < 0)
		return 0;

	int Usage = m_iBufferSize - (m_chpWritePos - m_chpReadPos);
	int CopiedSize = max(min(iSize, Usage), 0);
	m_chpWritePos += CopiedSize;
	m_iDataSize += CopiedSize;

	return CopiedSize;
}

int	StreamBuffer::MoveReadPos(int iSize)
{
	if (iSize < 0)
		return 0;

	if (iSize > m_iDataSize)
		return 0;

	m_chpReadPos += iSize;
	m_iDataSize -= iSize;

	if (m_iDataSize == 0)
		ClearBuffer();

	return iSize;
}

int	StreamBuffer::PeekData(char *chrSrc, int iSrcSize)
{
	int Usage = m_chpWritePos - m_chpReadPos;
	if (Usage == 0)
		return 0;

	int GetDataSize = max(min(iSrcSize, Usage), 0);

	memcpy_s(chrSrc, GetDataSize, m_chpReadPos, GetDataSize);

	return GetDataSize;
}
int	StreamBuffer::GetData(char *chpDest, int iSize)
{
	if (iSize > m_iDataSize)
	{
		ErrorCode = 5;
		return 0;
	}
	memcpy(chpDest, m_chpReadPos, iSize);
	m_chpReadPos += iSize;
	m_iDataSize -= iSize;


	return iSize;
}

int	StreamBuffer::PutData(char *chpSrc, int iSrcSize)
{
	if (m_chpWritePos + iSrcSize > m_chpBuffer + m_iBufferSize)
	{
		ErrorCode = 6;
		return 0;
	}

	memcpy(m_chpWritePos, chpSrc, iSrcSize);
	m_chpWritePos += iSrcSize;
	m_iDataSize += iSrcSize;

	return iSrcSize;
}

StreamBuffer& StreamBuffer::operator=(StreamBuffer &clSrStreamBuffer)
{
	if (m_chpBuffer != nullptr)
	{
		delete[] m_chpBuffer;
		m_chpBuffer = nullptr;
	}

	this->m_iDataSize = clSrStreamBuffer.m_iDataSize;
	this->m_iBufferSize = clSrStreamBuffer.m_iBufferSize;

	this->m_chpBuffer = new char[m_iBufferSize];

	memcpy(this->m_chpBuffer, clSrStreamBuffer.m_chpBuffer, m_iBufferSize);

	int ReadIndex = clSrStreamBuffer.m_chpReadPos - clSrStreamBuffer.m_chpBuffer;
	this->m_chpReadPos = m_chpBuffer + ReadIndex;

	int WriteIndex = clSrStreamBuffer.m_chpWritePos - clSrStreamBuffer.m_chpBuffer;
	this->m_chpWritePos = m_chpBuffer + WriteIndex;

	return *this;
}

BYTE StreamBuffer::MakeCheckSum()
{
	char *pIndex = m_chpReadPos;
	unsigned long long checkSum = 0;

	for (int i = 0; i < m_iDataSize; i++)
	{
		checkSum += *pIndex;
		pIndex++;
	}

	checkSum %= 256;

	return checkSum;
}


