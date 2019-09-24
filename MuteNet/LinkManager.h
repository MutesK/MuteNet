#pragma once

#include <memory>
#include <map>

namespace Network
{
	class Link;
	class TcpSocket;
	class ConnectPoint;
	class LinkManager
	{
	public:
		static std::shared_ptr<Link> make_shared(std::shared_ptr<TcpSocket>& socket, ConnectPoint& endPoint);
		static void disconnect_link(const std::shared_ptr<Link> link);
		
		static size_t UserSize();

		static std::map<Link*, std::shared_ptr<Link>>::iterator begin();
		static std::map<Link*, std::shared_ptr<Link>>::iterator end();
	};
}

