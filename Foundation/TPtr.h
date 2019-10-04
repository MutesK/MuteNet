#pragma once

#include "ReferenceCounter.h"

namespace Util
{
	namespace TL
	{
		template <class Type>
		class TPtr : public ReferenceCounter
		{
		private:
			Type* _Ptr;
			std::function<void(Type* obj)> _deletor;

			void decRef()
			{
				if (_Ptr && decRefCount() == 0)
				{
					// 여기 생각이 필요.
					_deletor(_Ptr);
				}
			}
		public:
			TPtr(std::function<void(Type* obj)>&& deletetor, Type* obj = nullptr)
				:_Ptr(obj), _deletor(deletetor)
			{
			}

			TPtr(TPtr<Type>& obj)
			{
				_Ptr = obj._Ptr;

				if (_Ptr)
				{
					obj->incRefCount();
				}
			}
			~TPtr<Type>()
			{
				decRefCount();
			}

			Type& operator*()
			{
				if (!_Ptr)
					throw;

				return *_Ptr;
			}

			Type* operator->()
			{
				if (!_Ptr)
					throw;

				return *_Ptr;
			}

			bool operator=(const TPtr<Type>& right)
			{
				return _Ptr == right._Ptr;
			}

			TPtr<Type>& operator=(TPtr<Type>& right) const
			{
				if (this == &right)
					return *this;

				if (right._Ptr)
					right.incRefCount();

				decRefCount();

				_Ptr = right._Ptr;
				return *this;
			}

			operator bool()
			{
				return _Ptr != nullptr;
			}

		};
	}
}