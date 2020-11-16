//
// Created by junmkim on 2020-11-13.
//

#ifndef MUTENET_SELECTIOCONTEXT_HPP
#define MUTENET_SELECTIOCONTEXT_HPP

#include "EventBaseComponent.hpp"
#include "SocketDescriptor.h"
#include "UnixLikeIOContextImpl.hpp"

#if defined(IOCONTEXT_SELECT)

namespace EventLoop
{
	class SelectIOContext : public IUnixLikeIOContextImpl
	{
		fd_set _ReadSet, _WriteSet;
	
		friend class UnixLikeSocketDescriptor;
		friend class LikeUnixListenerComponent;
	public:
		SelectIOContext ( IOContextEvent &Event,
		                  const uint32_t NumOfWorkerThread, const uint32_t Timeout );

	protected:
		virtual void DoWork ( ) override;

		void FDSet ( ContextContainer &Container );
		
		void FDIsSetAndCallback ( ContextContainer &Container );
	};
	
	
}

#endif


#endif //MUTENET_SELECTIOCONTEXT_HPP
