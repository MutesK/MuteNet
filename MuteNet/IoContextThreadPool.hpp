//
// Created by JunMin Kim on 2020/11/11.
//

#ifndef MUTENET_IOCONTEXTTHREADPOOL_HPP
#define MUTENET_IOCONTEXTTHREADPOOL_HPP

#include <SharedPool.hpp>
#include <Runnable.hpp>
#include <Event.hpp>

namespace EventLoop
{

    class IWorkItem
    {
    public:
        virtual void Do() = 0;
    };

    using WorkItemPtr = std::shared_ptr<IWorkItem>;

    using WorkPendingQueue = concurrency::concurrent_queue<WorkItemPtr>;

    class IOContextThreadPool final
    {
        Util::Event _TriggerEvent;
        bool _ClearFlag = false;
        uint32_t _Timeout;

        std::vector<std::thread> _ThreadPool;
        WorkPendingQueue _Queue;
    public:
        IOContextThreadPool() = default;

        IOContextThreadPool(const int32_t WorkerCount, const uint32_t Timeout);

        ~IOContextThreadPool();

        void EnqueueJob(const WorkItemPtr &Ptr);

    private:
        void InnerWork();
    };
}


#endif //MUTENET_IOCONTEXTTHREADPOOL_HPP
