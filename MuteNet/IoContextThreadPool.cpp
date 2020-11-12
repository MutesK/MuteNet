//
// Created by JunMin Kim on 2020/11/11.
//

#include "Common.h"
#include "IoContextThreadPool.hpp"

namespace EventLoop
{

    void IOContextThreadPool::InnerWork()
    {
        while (!_ClearFlag)
        {
            _TriggerEvent.Wait(_Timeout);

            if (_Queue.empty())
            {
                continue;
            }

            WorkItemPtr Ptr = nullptr;
            if (false == _Queue.try_pop(Ptr))
            {
                continue;
            }

            if (nullptr == Ptr)
            {
                continue;
            }

            Ptr->Do();
        }
    }

    IOContextThreadPool::IOContextThreadPool(const int32_t WorkerCount, const uint32_t Timeout)
            : _Timeout(Timeout)
    {
        _ThreadPool.resize(WorkerCount);

        for (int index = 0; index < WorkerCount; ++index)
        {
            _ThreadPool[index] = std::thread([&]()
                                             {
                                                 InnerWork();
                                             });
        }
    }

    IOContextThreadPool::~IOContextThreadPool()
    {
        _ClearFlag = true;

        _TriggerEvent.SetAll();

        std::for_each(_ThreadPool.begin(), _ThreadPool.end(), [](std::thread &thread)
        {
            thread.join();
        });
    }

    void IOContextThreadPool::EnqueueJob(const WorkItemPtr &Ptr)
    {
        _Queue.push(Ptr);

        _TriggerEvent.Set();
    }



}
