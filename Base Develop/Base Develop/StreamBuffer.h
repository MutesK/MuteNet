#pragma once

#include "../Foundation/foundation.h"

/*
	�ر򸮴� ���
	Serializer, Marshalling
*/

class StreamBuffer
{
private:
	enum en_PACKET
	{
		eBUFFER_DEFAULT = 2920,		// ��Ŷ�� �⺻ ���� ������.
	};

	void	Release(void);

	int		GetBufferSize(void) { return m_iBufferSize; }


	int		GetHeaderDataUseSize(void) 
	{
		return m_iDataSize;
	}

	BYTE MakeCheckSum();

private:
	unsigned __int64  BufferInit;
	//------------------------------------------------------------
	// ��Ŷ���� / ���� ������.
	//------------------------------------------------------------
	char*	m_chpBuffer;
	int		m_iBufferSize;

	//------------------------------------------------------------
	// ������ ���� ��ġ, ���� ��ġ.
	//------------------------------------------------------------
	char	*m_chpReadPos;
	char	*m_chpWritePos;

	//------------------------------------------------------------
	// ���� ���ۿ� ������� ������.
	//------------------------------------------------------------
	int		m_iDataSize;

	DWORD   ErrorCode;

	/////////////////////////////////////////////////////////////////////////////////////////
public:
	//////////////////////////////////////////////////////////////////////////
	// ������, �ı���.
	//
	// Return:
	//////////////////////////////////////////////////////////////////////////
	StreamBuffer();
	StreamBuffer(int iBufferSize);
	~StreamBuffer();

	//////////////////////////////////////////////////////////////////////////
	// ���� ������ ���.
	//
	// Parameters: ����.
	// Return: (char *)���� ������.
	//////////////////////////////////////////////////////////////////////////
	char	*GetBufferPtr(void) { return m_chpBuffer; }
	char	*GetReadBufferPtr(void) { return m_chpReadPos; }
	char	*GetWriteBufferPtr(void) { return m_chpWritePos; }


	/* ============================================================================= */
	// ������ ���۷�����.
	/* ============================================================================= */
	StreamBuffer	&operator = (StreamBuffer &clSrStreamBuffer);

	template<typename T>
	StreamBuffer& operator<<(T& Value)
	{
		PutData(reinterpret_cast<char *>(&Value), sizeof(T));
		return *this;
	}


	template<typename T>
	StreamBuffer& operator >> (T& Value)
	{
		GetData(reinterpret_cast<char *>(&Value), sizeof(T));
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	// ���� ������� ������ ���.
	//
	// Parameters: ����.
	// Return: (int)������� ����Ÿ ������.
	//////////////////////////////////////////////////////////////////////////
	int		GetUseSize(void) { return m_iDataSize; }

	//////////////////////////////////////////////////////////////////////////
	// ����Ÿ ���.
	//
	// Parameters: (char *)Dest ������. (int)Size.
	// Return: (int)������ ������.
	//////////////////////////////////////////////////////////////////////////
	int		GetData(char *chpDest, int iSize);

	//////////////////////////////////////////////////////////////////////////
	// ����Ÿ ����.
	//
	// Parameters: (char *)Src ������. (int)SrcSize.
	// Return: (int)������ ������.
	//////////////////////////////////////////////////////////////////////////
	int		PutData(char *chpSrc, int iSrcSize);


	int		PeekData(char *chrSrc, int iSrcSize);

	//////////////////////////////////////////////////////////////////////////
	// ��Ŷ û��.
	//
	// Parameters: ����.
	// Return: ����.
	//////////////////////////////////////////////////////////////////////////
	void	ClearBuffer(void);

	DWORD GetLastError()
	{
		return ErrorCode;
	}

	//////////////////////////////////////////////////////////////////////////
	// ���� Pos �̵�. (�����̵��� �ȵ�)
	// GetBufferPtr �Լ��� �̿��Ͽ� �ܺο��� ������ ���� ������ ������ ��� ���. 
	//
	// Parameters: (int) �̵� ������.
	// Return: (int) �̵��� ������.
	//////////////////////////////////////////////////////////////////////////
	int		MoveWritePos(int iSize);
	int		MoveReadPos(int iSize);
};


