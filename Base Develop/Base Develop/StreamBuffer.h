#pragma once

#include "../Foundation/foundation.h"

/*
	해깔리는 용어
	Serializer, Marshalling
*/

class StreamBuffer
{
private:
	enum en_PACKET
	{
		eBUFFER_DEFAULT = 2920,		// 패킷의 기본 버퍼 사이즈.
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
	// 패킷버퍼 / 버퍼 사이즈.
	//------------------------------------------------------------
	char*	m_chpBuffer;
	int		m_iBufferSize;

	//------------------------------------------------------------
	// 버퍼의 읽을 위치, 넣을 위치.
	//------------------------------------------------------------
	char	*m_chpReadPos;
	char	*m_chpWritePos;

	//------------------------------------------------------------
	// 현재 버퍼에 사용중인 사이즈.
	//------------------------------------------------------------
	int		m_iDataSize;

	DWORD   ErrorCode;

	/////////////////////////////////////////////////////////////////////////////////////////
public:
	//////////////////////////////////////////////////////////////////////////
	// 생성자, 파괴자.
	//
	// Return:
	//////////////////////////////////////////////////////////////////////////
	StreamBuffer();
	StreamBuffer(int iBufferSize);
	~StreamBuffer();

	//////////////////////////////////////////////////////////////////////////
	// 버퍼 포인터 얻기.
	//
	// Parameters: 없음.
	// Return: (char *)버퍼 포인터.
	//////////////////////////////////////////////////////////////////////////
	char	*GetBufferPtr(void) { return m_chpBuffer; }
	char	*GetReadBufferPtr(void) { return m_chpReadPos; }
	char	*GetWriteBufferPtr(void) { return m_chpWritePos; }


	/* ============================================================================= */
	// 연산자 오퍼레이터.
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
	// 현재 사용중인 사이즈 얻기.
	//
	// Parameters: 없음.
	// Return: (int)사용중인 데이타 사이즈.
	//////////////////////////////////////////////////////////////////////////
	int		GetUseSize(void) { return m_iDataSize; }

	//////////////////////////////////////////////////////////////////////////
	// 데이타 얻기.
	//
	// Parameters: (char *)Dest 포인터. (int)Size.
	// Return: (int)복사한 사이즈.
	//////////////////////////////////////////////////////////////////////////
	int		GetData(char *chpDest, int iSize);

	//////////////////////////////////////////////////////////////////////////
	// 데이타 삽입.
	//
	// Parameters: (char *)Src 포인터. (int)SrcSize.
	// Return: (int)복사한 사이즈.
	//////////////////////////////////////////////////////////////////////////
	int		PutData(char *chpSrc, int iSrcSize);


	int		PeekData(char *chrSrc, int iSrcSize);

	//////////////////////////////////////////////////////////////////////////
	// 패킷 청소.
	//
	// Parameters: 없음.
	// Return: 없음.
	//////////////////////////////////////////////////////////////////////////
	void	ClearBuffer(void);

	DWORD GetLastError()
	{
		return ErrorCode;
	}

	//////////////////////////////////////////////////////////////////////////
	// 버퍼 Pos 이동. (음수이동은 안됨)
	// GetBufferPtr 함수를 이용하여 외부에서 강제로 버퍼 내용을 수정할 경우 사용. 
	//
	// Parameters: (int) 이동 사이즈.
	// Return: (int) 이동된 사이즈.
	//////////////////////////////////////////////////////////////////////////
	int		MoveWritePos(int iSize);
	int		MoveReadPos(int iSize);
};


