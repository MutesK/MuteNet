//
// Created by JunMin Kim on 2020/11/11.
//

#ifndef MUTENET_IOCONTEXTIMPL_HPP
#define MUTENET_IOCONTEXTIMPL_HPP

#include "IoContextThreadPool.hpp"

namespace EventLoop
{
    class IOContextEvent;

    class IOContextImpl
    {
    protected:
        IOContextEvent &_Event;
        IOContextThreadPool _ThreadPool;
    public:
        IOContextImpl(IOContextEvent &Event,
                      const uint32_t NumOfWorkerThread, const uint32_t Timeout);
    };
}


#endif //MUTENET_IOCONTEXTIMPL_HPP
