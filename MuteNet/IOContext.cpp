#pragma once
#include "pch.h"
#include "Link.h"
#include "IOContext.h"
#include "Acceptor.h"
#include "EngineIO.hpp"
#include "IOService.h"
#include "LinkManager.h"
#include "ASyncWork.h"

using namespace Util;

namespace Network
{
	void SendContext::IOComplete(DWORD TransfferedBytes, void* CompletionKey)
	{
		auto SendQ = linkPtr->get_SendQ();

		{
			SAFE_UNIQUELOCK(SendQ->GetSharedLock());
			
			SendQ->MoveReadPostion(TransfferedBytes);
		}

		SendQ->set_isSending(false);
		EngineIO::OnSended(linkPtr, TransfferedBytes);
		
		delete this;
	}

	void RecvContext::IOComplete(DWORD TransfferedBytes, void* CompletionKey)
	{
		if (TransfferedBytes <= 0)
		{
			linkPtr->get_socket()->Shutdown(ShutdownBlockMode::BothBlock);
			LinkManager::removesession(linkPtr);
		}
		else
		{
			auto RecvQ = linkPtr->get_RecvQ();
			RecvQ->MoveWritePos(TransfferedBytes);
			
			uint32_t length = 0;

			while (true)
			{
				length = 0;

				if (RecvQ->GetUseSize() <= 0)
					break;

				RecvQ->Peek(&length, sizeof(uint32_t));

				if (RecvQ->GetUseSize() < (length + sizeof(uint32_t)))
					break;

				const auto packetSize = length + sizeof(uint32_t);
				auto HeapBlock = HeapBlock::make_shared();
				RecvQ->GetData(HeapBlock->GetBufferPtr(), packetSize);
				HeapBlock->MoveWritePosition(packetSize);

				auto InputStream = std::make_shared<InputMemoryStream>(HeapBlock);

				EngineIO::OnRecived(linkPtr, InputStream);
			}


			ASyncWork<RecvContext>::WorkRequest(linkPtr);
		}

		delete this;

	}

	void AcceptContext::IOComplete(DWORD TransfferedBytes, void* CompletionKey, IOService* Service)
	{
		auto acceptor = reinterpret_cast<Acceptor*>(CompletionKey);
		auto socket = linkPtr->get_socket();

		socket->SetUpdateAcceptContext(acceptor->get_listen()->socket_handle());
		socket->SetNagle(true);

		Service->RegisterHandle(socket->native_handle(), nullptr);

		EngineIO::OnAccepted(linkPtr);

		ASyncWork<RecvContext>::WorkRequest(linkPtr);
		acceptor->PostAccept();
		delete this;

	}

	void ConnectContext::IOComplete(DWORD TransfferedBytes, void* CompletionKey)
	{
		auto socket = linkPtr->get_socket();

		socket->SetUpdateConnectContext();
		socket->SetNagle(true);

		EngineIO::OnConnected(linkPtr);

		ASyncWork<RecvContext>::WorkRequest(linkPtr);

		delete this;
	}
}