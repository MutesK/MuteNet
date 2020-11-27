//
// Created by JunMin Kim on 2020/11/11.
//

#include "Common.h"
#include "Event.hpp"

namespace Util
{
	Event::Event ( )
			: _shouldContinue ( false )
	{
	}
	
	void Event::Wait ( )
	{
		std::unique_lock<std::mutex> lock ( _mutex );
		
		_condVar.wait ( lock, [ this ] ( )
		{
			return _shouldContinue;
		} );
		_shouldContinue = false;
	}
	
	void Event::Set ( )
	{
		{
			std::unique_lock<std::mutex> lock ( _mutex );
			_shouldContinue = true;
		}
		
		_condVar.notify_one ( );
	}
	
	void Event::SetAll ( )
	{
		{
			std::unique_lock<std::mutex> lock ( _mutex );
			_shouldContinue = true;
		}
		
		_condVar.notify_all ( );
	}
	
	bool Event::Wait ( uint32_t TimeoutMsec )
	{
		auto dst = std::chrono::system_clock::now ( ) + std::chrono::milliseconds ( TimeoutMsec );
		
		std::unique_lock<std::mutex> lock ( _mutex );
		bool result = _condVar.wait_until ( lock, dst, [ this ] ( )
		{
			return _shouldContinue;
		} );
		_shouldContinue = false;
		
		return result;
	}
}