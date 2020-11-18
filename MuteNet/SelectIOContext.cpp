//
// Created by junmkim on 2020-11-13.
//




#include "Common.h"
#include "TypeDefine.hpp"
#include "SelectIOContext.hpp"
#include "Descriptor.h"
#include "LikeUnixListenerComponent.h"

#if defined(IOCONTEXT_SELECT)

namespace EventLoop
{
	
	SelectIOContext::SelectIOContext ( IOContextEvent &Event, const uint32_t NumOfWorkerThread, const uint32_t Timeout )
			: IUnixLikeIOContextImpl ( Event, NumOfWorkerThread, Timeout )
	{
	
	}
	
	void SelectIOContext::DoWork ( )
	{
		socket_t MaxDescriptorNumber = 0;
		ContextContainer Container;
		
		struct timeval tv;
		tv.tv_sec = _Timeout;
		
		while ( !IsStop ( ))
		{
			Container = _Container;
			
			FD_ZERO( &_ReadSet );
			FD_ZERO( &_WriteSet );
			
			FDSet ( Container );
			
			int ndfs = Container._RegisteredSockets.size ( );
			int result = select ( ndfs, &_ReadSet, &_WriteSet, nullptr, &tv );
			
			if ( result == -1 || result == 0 )
			{
				continue;
			}
			
			FDIsSetAndCallback ( Container );
		}
	}

	void SelectIOContext::FDSet ( ContextContainer &Container )
	{
		std::for_each ( Container._RegisteredSockets.begin ( ), Container._RegisteredSockets.end ( ),
		                [ & ] ( DescriptorPtr &Ptr )
		                {
			                FD_SET( Ptr->_descriptor, &_ReadSet );
			                FD_SET( Ptr->_descriptor, &_WriteSet );
		                } );
	}
	
	void SelectIOContext::FDIsSetAndCallback ( SelectIOContext::ContextContainer &Container )
	{
		std::for_each ( Container._RegisteredSockets.begin ( ), Container._RegisteredSockets.end ( ),
		                [ & ] ( DescriptorPtr &Ptr )
		                {
			                const auto &ThreadPool = GetThreadPool ( );
			                const static auto DispatchSignal = [ & ] ( )
			                {
				                if ( FD_ISSET( Ptr->_descriptor, &_ReadSet ))
				                {
				                	if(!Ptr->_Read())
				                		return;
				                	
					                Ptr->_ReadCallback ( Ptr, Ptr->_Key );
				                }
				                if ( FD_ISSET( Ptr->_descriptor, &_WriteSet ))
				                {
				                	if(!Ptr->_Send())
				                		return;
				                	
					                Ptr->_WriteCallback ( Ptr, Ptr->_Key );
				                }
			                };
			
			                ThreadPool->EnqueueJob ( DispatchSignal );
		                } );
	}

}

#endif