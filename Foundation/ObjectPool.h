#pragma once
#pragma warning(disable : 6550)

#include <mutex>
#include <map>
#include <stack>
#include <functional>

namespace Util
{
	namespace TL
	{
		template <typename Type>
		class ObjectPool
		{
		private:
			std::mutex							_mutex;
			std::map<void*, void*>				_elementPool;
			std::stack<void*>					_unUsedIndexs;

			std::function<void(Type* ptr)>		_customDeletor;

			std::size_t							_allocPoolSize;
			std::atomic_bool					_dynamicAllocFlag;
		public:
			ObjectPool(size_t PoolSize = 10000);
			~ObjectPool();

			void setDynamicAlloc(bool flag)
			{
				_dynamicAllocFlag = flag;
			}

			template <typename ...Args>
			std::weak_ptr<Type> make_weak(Args&&... arguments);

			template <typename ...Args>
			std::shared_ptr<Type> make_shared(Args&& ... arguments);

			template <typename ...Args>
			std::unique_ptr<Type> make_unique(Args&& ... arguments);

			template <typename ...Args>
			Type* operator()(Args&& ... arguments)
			{
				return Alloc(arguments...);
			}

			template <typename ...Args>
			Type* Alloc(Args&&...arguments);

			void Free(Type* ptr);

			size_t UseCount() const
			{
				return _elementPool.size() - _unUsedIndexs.size();
			}
		private:
			void AllocNode()
			{
				if (!_dynamicAllocFlag)
					return;

				std::lock_guard<std::mutex> lock(_mutex);

				for (size_t index = 0; index < _allocPoolSize; ++index)
				{
					auto object = std::malloc(sizeof(Type));
					_elementPool[object] = object;

					_unUsedIndexs.push(object);
				}
			}
		};

		template <typename Type>
		ObjectPool<Type>::ObjectPool(size_t PoolSize)
			:_allocPoolSize(PoolSize)
		{
			_customDeletor = [&](Type* ptr)
			{
				this->Free(ptr);
			};

			std::lock_guard<std::mutex> lock(_mutex);

			for (size_t index = 0; index < _allocPoolSize; ++index)
			{
				auto object = std::malloc(sizeof(Type));
				_elementPool[object] = object;

				_unUsedIndexs.push(object);
			}
		}

		template <typename Type>
		ObjectPool<Type>::~ObjectPool()
		{
			for(auto object : _elementPool)
			{
				std::free(object.first);
			}

			_elementPool.clear();
		}

		template <typename Type>
		template <typename ... Args>
		std::weak_ptr<Type> ObjectPool<Type>::make_weak(Args&&... arguments)
		{
			auto rawPtr = Alloc(arguments...);

			return std::weak_ptr<Type>(rawPtr, _customDeletor);
		}

		template <typename Type>
		template <typename ... Args>
		std::shared_ptr<Type> ObjectPool<Type>::make_shared(Args&& ... arguments)
		{
			auto rawPtr = Alloc(arguments...);

			return std::shared_ptr<Type>(rawPtr, _customDeletor);
		}

		template <typename Type>
		template <typename ... Args>
		std::unique_ptr<Type> ObjectPool<Type>::make_unique(Args&& ... arguments)
		{
			auto rawPtr = Alloc(arguments...);

			return std::unique_ptr<Type>(rawPtr, _customDeletor);
		}

		template <typename Type>
		template <typename ... Args>
		Type* ObjectPool<Type>::Alloc(Args&&... arguments)
		{
			if(_unUsedIndexs.size() <= 0 && !_dynamicAllocFlag)
			{
				return nullptr;
			}

			if (_unUsedIndexs.size() <= 0 && _dynamicAllocFlag)
			{
				AllocNode();
			}

			void* ptr = nullptr;

			{
				std::lock_guard<std::mutex> lock(_mutex);

				ptr = _unUsedIndexs.top();
				_unUsedIndexs.pop();
			}

			new (ptr) Type(arguments...);

			return reinterpret_cast<Type *>(ptr);
		}

		template <typename Type>
		void ObjectPool<Type>::Free(Type* ptr)
		{
			{
				std::lock_guard<std::mutex> lock(_mutex);

				auto iter = _elementPool.find(ptr);
				if (iter == _elementPool.end())
					throw;

				_unUsedIndexs.push(ptr);
			}

			ptr->~Type();
		}
	}
}
