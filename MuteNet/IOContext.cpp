#pragma once
#include "pch.h"
#include "Link.h"
#include "LinkManager.h"
#include "IOContext.h"
#include "Acceptor.h"
#include "EngineIO.hpp"

using namespace Util;

namespace Network
{
	Util::TL::ObjectPool<SendContext> SendContext::OverlappedPool;
	Util::TL::ObjectPool<RecvContext> RecvContext::OverlappedPool;

	void SendContext::IOComplete(DWORD TransfferedBytes, void* CompletionKey)
	{
		auto& SendQ = linkPtr->get_SendQ();

		{
			std::lock_guard<std::recursive_mutex> lock(SendQ->_mutex);

			SendQ->MoveReadPostion(TransfferedBytes);
		}

		EngineIO::OnSended(linkPtr, TransfferedBytes);

		linkPtr->set_isSend(true);
		SendContext::OverlappedPool.Free(this);
	}

	void RecvContext::IOComplete(DWORD TransfferedBytes, void* CompletionKey)
	{
		if (TransfferedBytes <= 0)
		{
			linkPtr->get_socket()->Shutdown(ShutdownBlockMode::BothBlock);
			LinkManager::disconnect_link(linkPtr);
		}
		else
		{
			auto& RecvQ = linkPtr->get_RecvQ();
			RecvQ->MoveWritePostion(TransfferedBytes);
			
			uint32_t length = 0;

			while (true)
			{
				length = 0;
				RecvQ->GetData(&length, sizeof(length));

				if (RecvQ->GetUsedSize() < length)
					break;

				const auto bufferPtr = RecvQ->GetReadBufferPtr();
				
				auto bufferStream = std::make_shared<InputMemoryStream>(bufferPtr, length);
				RecvQ->MoveReadPostion(length);

				EngineIO::OnRecived(linkPtr, bufferStream);
			}


			linkPtr->RecvPost();
		}

		RecvContext::OverlappedPool.Free(this);
	}

}