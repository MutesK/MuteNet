#pragma once

#include "ASyncIORequest.h"
#include "Link.h"
#include "Socket.h"

namespace MuteNet
{
	class ASyncSendRequest : public ASyncIORequest
	{
		LinkPtr link;
		std::shared_ptr<Socket> socket;
		char* inData = nullptr;
		size_t length = 0;
		ALink::CallbacksPtr callbackPtr;
	public:
		ASyncSendRequest(LinkPtr link, std::shared_ptr<Socket> socket, const char* inData, size_t len,
			const ALink::CallbacksPtr& callbackPtr);
		virtual ~ASyncSendRequest();

		static ASyncSendRequest* GetSendRequest(LinkPtr link, std::shared_ptr<Socket> socket, const char* inData, size_t len,
			const ALink::CallbacksPtr& callbackPtr);
		static void FreeSendRequest(ASyncSendRequest* request);

		void OnComplete(uint32_t transferredBytes, IOOperation op) override;
		void OnError(IOOperation op) override;

		bool Process() override;
	};
}

