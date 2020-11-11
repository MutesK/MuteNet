//
// Created by JunMin Kim on 2020/11/11.
//

#ifndef MUTENET_RUNNABLE_HPP
#define MUTENET_RUNNABLE_HPP


#include <atomic>
#include <thread>


#if defined(WIN32) || defined(WIN64)
#include <Windows.h>
#else

#include <pthread.h>
#include <signal.h>

typedef pthread_t native_handle_type;

void SuspendThread(native_handle_type native_handle)
{
    pthread_kill(native_handle, SIGUSR1);
}

void ResumeThread(native_handle_type native_handle)
{
    pthread_kill(native_handle, SIGUSR2);
}

#endif
namespace Util
{
    class Runnable
    {
        std::atomic_bool _Stop;
        std::thread _Thread;

    public:
        Runnable()
                : _Stop(), _Thread()
        {
        }

        virtual ~Runnable()
        {
            Stop();
        }

        Runnable(Runnable const &) = delete;

        Runnable &operator=(Runnable const &) = delete;


        void Start()
        {
            _Thread = std::thread(&Runnable::DoWork, this);
        }

        void Stop()
        {
            _Stop = true;
            _Thread.join();
        }

        void Suspend()
        {
            SuspendThread(_Thread.native_handle());
        }

        void Resume()
        {
            ResumeThread(_Thread.native_handle());
        }

        bool IsStop() const
        {
            return _Stop;
        }

    protected:
        virtual void DoWork() = 0;
    };
}


#endif //MUTENET_RUNNABLE_HPP
