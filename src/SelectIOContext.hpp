//
// Created by junmkim on 2020-11-13.
//

#ifndef MUTENET_SELECTIOCONTEXT_HPP
#define MUTENET_SELECTIOCONTEXT_HPP

#include "EventBaseComponent.hpp"
#include "Descriptor.h"
#include "UnixLikeIOContextImpl.hpp"

#ifdef POSIX_LIKE

namespace EventLoop
{
	class SelectIOContext : public IUnixLikeIOContextImpl
	{
		fd_set _ReadSet, _WriteSet;

	public:
		SelectIOContext(IOContextEvent& Event,
			const uint32_t NumOfWorkerThread, const uint32_t Timeout);

	protected:
		virtual void DoWork() override;
	};


}

#endif
#endif //MUTENET_SELECTIOCONTEXT_HPP
