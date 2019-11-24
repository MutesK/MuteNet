#pragma once


namespace MuteNet
{
	typedef LPFN_ACCEPTEX						AcceptExPtr;
	typedef LPFN_CONNECTEX						ConnectExPtr;
	typedef LPFN_GETACCEPTEXSOCKADDRS			GetAcceptExSockAddrsPtr;

	struct ExtensionFunctions
	{
		AcceptExPtr					_AcceptEx;
		ConnectExPtr				_ConnectEx;
		GetAcceptExSockAddrsPtr		_GetAcceptExSockaddrs;
	};

	inline void* GetExtensionFunction(SOCKET s, const GUID* fn)
	{
		void* ptr = nullptr;
		DWORD bytes = 0;

		auto ret = std::invoke(WSAIoctl, s, SIO_GET_EXTENSION_FUNCTION_POINTER,
			(GUID*)fn, sizeof(*fn), &ptr, sizeof(ptr), &bytes, nullptr, nullptr);

		assert(ret != SOCKET_ERROR);

		return ptr;
	}

	inline void InitExtensionFunctions(struct ExtensionFunctions* ext)
	{
		const GUID acceptex = WSAID_ACCEPTEX;
		const GUID connectex = WSAID_CONNECTEX;
		const GUID getaccpetexsockaddrs = WSAID_GETACCEPTEXSOCKADDRS;

		SOCKET serviceProvider = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

		if (serviceProvider == INVALID_SOCKET)
			return;

		ext->_AcceptEx = 
			reinterpret_cast<AcceptExPtr>(GetExtensionFunction(serviceProvider, &acceptex));
		ext->_ConnectEx =
			reinterpret_cast<ConnectExPtr>(GetExtensionFunction(serviceProvider, &connectex));
		ext->_GetAcceptExSockaddrs =
			reinterpret_cast<GetAcceptExSockAddrsPtr>(GetExtensionFunction(serviceProvider, &getaccpetexsockaddrs));


		// closesocket(serviceProvider);
	}
}
