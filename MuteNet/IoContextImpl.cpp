//
// Created by JunMin Kim on 2020/11/11.
//

#include "Common.h"
#include "TypeDefine.hpp"
#include "IoContextImpl.hpp"

namespace EventLoop
{
    IOContextImpl::IOContextImpl(IOContextEvent &Event,
                                 const uint32_t NumOfWorkerThread,
                                 const uint32_t Timeout)
            : _Event(Event), _Timeout(Timeout)
    {
        _ThreadPool = std::make_shared<IOContextThreadPool>(NumOfWorkerThread);
    }
    
    std::shared_ptr<IOContextThreadPool> &IOContextImpl::GetThreadPool ( )
    {
        return _ThreadPool;
    }
}
