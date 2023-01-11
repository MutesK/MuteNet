//
// Created by Mute on 2020-11-18.
//

#include "Common.h"
#include "TypeDefine.hpp"
#include "TCPLink.hpp"

using namespace MuteNet;

TCPLink::CallbacksPtr TCPLink::GetCallbacks()
{
	return _Callback;
}
