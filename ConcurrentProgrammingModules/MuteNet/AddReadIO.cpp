#include "AddReadIO.h"
#include "TcpSocket.h"


namespace Network
{
	AddReadIO::AddReadIO(Link::Handle link)
		:_Link(link)
	{
	}

	AddReadIO::~AddReadIO()
	{
		delete[] _buffers;
	}

	void AddReadIO::ReadRequest(int ReadBytes)
	{
		_buffers = new char[ReadBytes]; 

		WSABUF wsabuf;
		wsabuf.buf = _buffers;
		wsabuf.len = ReadBytes;

		_Link->_socket->OverlappedIORecv(&wsabuf, ReadBytes, this);
	}

	void AddReadIO::ReadProcess(uint32_t TransferredSize, std::shared_ptr<MemoryStream>& Stream)
	{
		auto outStream = std::dynamic_pointer_cast<OutputMemoryStream>(Stream);

		outStream->Write(_buffers, TransferredSize);
	}
}
