#pragma once

#include "pch.h"
#include "../Foundation/Singleton.h"


namespace MuteNet
{
	class Link;
	typedef std::shared_ptr<Link> LinkPtr;
	typedef std::vector<LinkPtr>  LinkPtrs;

	class ServerHandle;
	typedef std::shared_ptr<ServerHandle> ServerHandlePtr;
	typedef std::vector<ServerHandlePtr>  ServerHandlePtrs;

	class NetworkManager : public Util::TL::Singleton<NetworkManager>
	{
	private:
		SafeSharedLock				_Lock;
		ServerHandlePtrs			_Servers;
		LinkPtrs					_Links;
	public:
		void Initialize(void);

		void Terminate(void);

		void AddLink(LinkPtr link);

		void RemoveLink(const Link* link);

		void AddServer(ServerHandlePtr Server);

		void RemoveServer(const ServerHandle* Server);
	};

}