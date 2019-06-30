#pragma once


#include "MuteNetFoundation.h"
#include "../MemoryStream/OutputMemoryStream.h"
#include "Link.h"


/*
 *	��ũ�� Recv or File Read ��û�� ���� IO
 *
 *	IO Completetion -> MemoryStream Write - > User Range Callback
 *
 */

class MemoryStream;

namespace Network {

	class TcpSocket;
	class AddReadIO
	{
	public:
		OVERLAPPED _ReadIO;
		char* _buffers;  // POOL REQUIRED
		Link::Handle _Link;

		explicit AddReadIO(Link::Handle link);
		~AddReadIO();

		void ReadRequest(int ReadBytes);
		void ReadProcess(uint32_t TransferredSize, std::shared_ptr<MemoryStream>& Stream);
	};

}
