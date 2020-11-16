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
	class IRunnable
	{
	public:
		virtual void Start ( ) = 0;
		
		virtual void Stop ( ) = 0;
		
		virtual bool IsStop ( ) const = 0;
	};
	
	class Runnable
	{
		std::atomic_bool _Stop;
		std::thread _Thread;
	
	public:
		Runnable ( )
				: _Stop ( ), _Thread ( )
		{
		}
		
		virtual ~Runnable ( )
		{
			Stop ( );
		}
		
		Runnable ( Runnable const & ) = delete;
		
		Runnable &operator= ( Runnable const & ) = delete;
		
		
		virtual void Start ( ) override
		{
			_Thread = std::thread ( &Runnable::DoWork, this );
		}
		
		virtual void Stop ( ) override
		{
			_Stop = true;
			_Thread.join ( );
		}
		
		void Suspend ( )
		{
			SuspendThread ( _Thread.native_handle ( ));
		}
		
		void Resume ( )
		{
			ResumeThread ( _Thread.native_handle ( ));
		}
		
		virtual bool IsStop ( ) const override
		{
			return _Stop;
		}
	
	protected:
		virtual void DoWork ( ) = 0;
	};
}


#endif //MUTENET_RUNNABLE_HPP
