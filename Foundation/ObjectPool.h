#pragma once
#pragma warning(disable : 6550)

#include <mutex>
#include <set>
#include <stack>
#include <queue>
#include <functional>

namespace Util
{
	namespace TL
	{
		template <typename Type, bool CallCtor = true, bool DynamicAllocateMode = false>
		class ObjectPool
		{
		private:
			std::mutex							_mutex;
			std::set<void*>						_elementPool;
			std::queue<void*>					_unUsedIndexs;

			std::function<void(Type* ptr)>		_customDeletor;
			std::size_t							_PoolSize;
		public:
			ObjectPool(size_t PoolSize = 10000);
			~ObjectPool();

			template <typename ...Args>
			Type* operator()(Args&& ... arguments)
			{
				return Alloc(std::forward<Args>(arguments)...);
			}

			template <typename ...Args>
			Type* Alloc(Args&&...arguments);

			void Free(const Type* ptr);

			size_t UseCount() const
			{
				return _elementPool.size() - _unUsedIndexs.size();
			}
		};

		template <typename Type, bool CallCtor, bool DynamicAllocateMode>
		ObjectPool<Type, CallCtor, DynamicAllocateMode>::ObjectPool(size_t PoolSize)
			:_PoolSize(PoolSize)
		{
			_customDeletor = [&](Type* ptr)
			{
				this->Free(ptr);
			};

			for(size_t index = 0; index < PoolSize; ++index)
			{
				auto object = std::malloc(sizeof(Type));
				assert(object != nullptr);

				_elementPool.insert(object);
				_unUsedIndexs.push(object);
			}
		}

		template <typename Type, bool CallCtor, bool DynamicAllocateMode>
		ObjectPool<Type, CallCtor, DynamicAllocateMode>::~ObjectPool()
		{
			for(auto object : _elementPool)
			{
				std::free(object);
			}

			_elementPool.clear();
		}

		template <typename Type, bool CallCtor, bool DynamicAllocateMode>
		template <typename ... Args>
		Type* ObjectPool<Type, CallCtor, DynamicAllocateMode>::Alloc(Args&&... arguments)
		{
			if(_unUsedIndexs.size() <= 0)
			{
				if (DynamicAllocateMode && _elementPool.size() < 50000) 
				{
					for (size_t index = 0; index < _PoolSize; ++index)
					{
						auto object = std::malloc(sizeof(Type));
						assert(object != nullptr);

						{
							std::lock_guard<std::mutex> lock(_mutex);

							_elementPool.insert(object);
							_unUsedIndexs.push(object);
						}
					}
				}
				else
					return nullptr;
			}

			void* ptr = nullptr;

			{
				std::lock_guard<std::mutex> lock(_mutex);

				ptr = _unUsedIndexs.front();
				_unUsedIndexs.pop();
			}

			if(CallCtor)
				new (ptr) Type(std::forward<Args>(arguments)...);

			return reinterpret_cast<Type *>(ptr);
		}

		template <typename Type, bool CallCtor, bool DynamicAllocateMode>
		void ObjectPool<Type, CallCtor, DynamicAllocateMode>::Free(const Type* ptr)
		{
			auto pointer = const_cast<Type*>(ptr);

			{
				std::lock_guard<std::mutex> lock(_mutex);

				auto iter = _elementPool.find(pointer);
				if (iter == _elementPool.end())
					throw;

				_unUsedIndexs.push(pointer);
			}

			if (CallCtor)
			{
				pointer->~Type();
			}
		}
	}
}
