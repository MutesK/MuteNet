#include "ASyncSendRequest.h"
#include "Allocator.h"

namespace MuteNet
{
	ASyncSendRequest::ASyncSendRequest(LinkPtr link, std::shared_ptr<Socket> socket, const char* inData, size_t len,
		const ALink::CallbacksPtr& callbackPtr)
		: link(std::move(link)), socket(std::move(socket)), length(len), callbackPtr(callbackPtr)
	{
		this->inData = static_cast<char*>(Allocator::GetResource().allocate(length));
		std::memcpy(this->inData, inData, length);
	}

	ASyncSendRequest::~ASyncSendRequest()
	{
		if (inData)
		{
			Allocator::GetResource().deallocate(inData, length);
			inData = nullptr;
		}
	}

	ASyncSendRequest* ASyncSendRequest::GetSendRequest(LinkPtr link, std::shared_ptr<Socket> socket, const char* inData,
		size_t len, const ALink::CallbacksPtr& callbackPtr)
	{
		return Allocator::New<ASyncSendRequest>(link, socket, inData, len, callbackPtr);
	}

	void ASyncSendRequest::FreeSendRequest(ASyncSendRequest* request)
	{
		Allocator::Delete(request);
	}

	bool ASyncSendRequest::Process()
	{
		if (!socket || !socket->IsValid())
			return false;

		return socket->ASyncSend(inData, length, GetPlatformContext());
	}

	void ASyncSendRequest::OnComplete(uint32_t transferredBytes, IOOperation op)
	{
		FreeSendRequest(this);
	}

	void ASyncSendRequest::OnError(IOOperation op)
	{
		FreeSendRequest(this);

	}

}