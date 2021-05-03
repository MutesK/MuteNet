//
// Created by Mute on 2020-11-18.
//

#ifndef MUTENET_SERVERHANDLE_HPP
#define MUTENET_SERVERHANDLE_HPP

namespace MuteNet
{
	class ServerHandle
	{
		friend class Network;
	public:
		virtual ~ServerHandle() {}

		virtual void Close() = 0;

		virtual bool IsListening() const = 0;
	};
}

#endif //MUTENET_SERVERHANDLE_HPP
