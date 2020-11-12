//
// Created by junmkim on 2020-11-12.
//

#ifndef MUTENET_WIN32LISTENERCOMPONENT_H
#define MUTENET_WIN32LISTENERCOMPONENT_H

#include "ListenerComponent.hpp"

#if defined(WIN32)

namespace EventLoop
{
    class Win32ListenerComponent : public ListenerComponent
    {

    };
}

#endif
#endif //MUTENET_WIN32LISTENERCOMPONENT_H
