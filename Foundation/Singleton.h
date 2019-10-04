#pragma once

#include "pch.h"

namespace Util
{
	namespace TL
	{
		template <typename Type>
		class Singleton
		{
		public:
			static Type& Get()
			{
				static Type StaticObject;
				return StaticObject;
			}

			NON_COPYABLE(Singleton);
		};
	}
}