//
// Created by JunMin Kim on 2020/11/11.
//

#ifndef MUTENET_SHAREDPOOL_HPP
#define MUTENET_SHAREDPOOL_HPP

#include <cassert>
#include <vector>
#include <memory>

namespace Util
{
	template<class Type>
	class SharedPool
	{
		std::vector<std::shared_ptr<Type>> _Pool;
	public:
		SharedPool ( ) = default;
		
		virtual ~SharedPool ( ) = default;
		
		void Add ( std::shared_ptr<Type> Ptr )
		{
			_Pool.emplace_back ( Ptr );
		}
		
		virtual std::shared_ptr<Type> Acquire ( )
		{
			assert( ~Empty ( ));
			return *std::find_if ( _Pool.begin ( ), _Pool.end ( ), [ ] ( const std::shared_ptr<Type> &Ptr )
			{
				return Ptr.use_count ( ) == 1;
			} );
		}
		
		virtual bool Empty ( ) const
		{
			return std::none_of ( _Pool.begin ( ), _Pool.end ( ), [ ] ( const std::shared_ptr<Type> &Ptr )
			{
				return Ptr.use_count ( ) == 1;
			} );
		}
	};
}

#endif //MUTENET_SHAREDPOOL_HPP
