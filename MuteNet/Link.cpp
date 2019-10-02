#include "pch.h"
#include "Link.h"
#include "IOContext.h"
#include "ASyncWork.h"


using namespace Util;
namespace Network
{
	Link::Link()
	{
		_socket = std::make_unique<TcpSocket>(AF_INET);

		_RecvQ = CircularBuffer::Alloc();
		_SendQ = CircularBuffer::Alloc();
	}

	Link::~Link()
	{
		std::cout << "Destory LINK ERROR!!!!!\n";
	}




}
