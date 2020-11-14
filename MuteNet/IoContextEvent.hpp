//
// Created by JunMin Kim on 2020/11/11.
//

#ifndef MUTENET_IOCONTEXTEVENT_HPP
#define MUTENET_IOCONTEXTEVENT_HPP

namespace EventLoop
{
	class IOContextImpl;
	
	using IOContextImplPtr = std::shared_ptr<IOContextImpl>;
	
	class IOContextEvent final
	{
		IOContextImplPtr _ContextImpl;
	public:
		IOContextEvent ( const uint32_t NumOfWorkerThread, const uint32_t Timeout );
		
		~IOContextEvent ( );
		
		
	};
}


#endif //MUTENET_IOCONTEXTEVENT_HPP
