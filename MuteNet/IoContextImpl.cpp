//
// Created by JunMin Kim on 2020/11/11.
//

#include "Common.h"
#include "IoContextImpl.hpp"

namespace EventLoop
{

    void WorkPendingQueue::Enqueue(const WorkItemPtr &WorkPtr)
    {
        std::unique_lock<std::shared_mutex> lock(_Mutex);

        _EnqueueOnlyQueue.push(WorkPtr);
    }

    std::queue<WorkItemPtr> WorkPendingQueue::GetWorkQueue()
    {
        std::queue<WorkItemPtr> CopyWorkQueue;

        std::unique_lock<std::shared_mutex> lock(_Mutex);
        CopyWorkQueue.swap(_EnqueueOnlyQueue);

        return CopyWorkQueue;
    }


    IOContextImpl::IOContextImpl(IOContextEvent &Event,
                                 const uint32_t NumOfWorkerThread,
                                 const uint32_t Timeout)
            : _Event(Event)
    {
        _ThreadPool = IOContextThreadPool(NumOfWorkerThread);
    }
}
