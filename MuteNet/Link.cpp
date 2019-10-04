#include "pch.h"
#include "Link.h"
#include "IOContext.h"
#include "EngineIO.hpp"
#include "LinkManager.h"

using namespace Util;
namespace Network
{
	Link::Link()
	{
		_SendContext.linkPtr = this;
		_RecvContext.linkPtr = this;
	}

	Link::~Link()
	{
		std::cout << "Destory LINK ERROR!!!!!\n";
	}

	void Link::SendPost()
	{
		WSABUF wsabuf[2];
		int bufcount;

		while (_SendQ.get_isSending())
			continue;

		_SendQ.set_isSending(true);

		{
			SAFE_SHAREDLOCK(_SendQ.GetSharedLock());

			bufcount = _SendQ.GetReadBufferAndLengths(&wsabuf[0].buf, (uint32_t&)wsabuf[0].len,
				&wsabuf[1].buf, (uint32_t&)wsabuf[1].len);
		}

		if(_socket.OverlappedIOSend(wsabuf, bufcount, &_SendContext.Overlapped))
			incRefCount();
	}

	void Link::RecvPost()
	{
		WSABUF wsabuf[2];
		int bufcount;

		bufcount = _RecvQ.GetWriteBufferAndLengths(&wsabuf[0].buf, (uint32_t&)wsabuf[0].len,
			&wsabuf[1].buf, (uint32_t&)wsabuf[1].len);

		if(_socket.OverlappedIORecv(wsabuf, bufcount, &_RecvContext.Overlapped))
			incRefCount();
	}

	void Link::SendPacket(const std::shared_ptr<Util::OutputMemoryStream>& Packet)
	{
		{
			SAFE_UNIQUELOCK(_SendQ.GetSharedLock());

			_SendQ.PutData(Packet->GetBufferPtr(), Packet->GetLength());
		}

		SendPost();

	}
	void Link::SendCompleteIO(DWORD TransfferedBytes, void* CompletionKey)
	{
		{
			SAFE_UNIQUELOCK(_SendQ.GetSharedLock());

			_SendQ.MoveReadPostion(TransfferedBytes);
		}

		_SendQ.set_isSending(false);
		EngineIO::OnSended(this, TransfferedBytes);

		decRefCount();
	}
	void Link::RecvCompleteIO(DWORD TransfferedBytes, void* CompletionKey)
	{
		if (TransfferedBytes <= 0)
		{
			_socket.Shutdown(ShutdownBlockMode::BothBlock);
		}
		else
		{
			_RecvQ.MoveWritePos(TransfferedBytes);

			uint32_t length = 0;

			while (true)
			{
				length = 0;

				if (_RecvQ.GetUseSize() <= 0)
					break;

				_RecvQ.Peek(&length, sizeof(uint32_t));

				if (_RecvQ.GetUseSize() < (length + sizeof(uint32_t)))
					break;

				const auto packetSize = length + sizeof(uint32_t);
				auto HeapBlock = HeapBlock::Alloc();
				_RecvQ.GetData(HeapBlock->GetBufferPtr(), packetSize);
				HeapBlock->MoveWritePosition(packetSize);

				auto InputStream = std::make_shared<InputMemoryStream>(HeapBlock);
				
				EngineIO::OnRecived(this, InputStream);
			}

			RecvPost();
		}

		decRefCount();
	}

	void Link::decRefCount()
	{
		Util::ReferenceCounter::decRefCount();

		if (_refcount <= 0)
		{
			LinkManager::removeSession(this);
		}
	}
}
