#pragma once

#include "Common.h"

namespace Util
{
	namespace TL
	{
		template<typename Type>
		class Singleton
		{
		public:
			static Type &Get ( )
			{
				static Type StaticObject;
				return StaticObject;
			}
		};
	}
}