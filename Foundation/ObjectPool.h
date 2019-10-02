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
		class UniqueCustomDeleter
		{
		public:
			static std::unique_ptr<Type, std::function<void(Type*)>> DeletorType;
		};

		template <typename Type, bool CallCtor = true, bool DynamicAllocateMode = false>
		class ObjectPool
		{
		private:
			std::mutex							_mutex;
			std::map<void*, void*>				_elementPool;
			std::stack<void*>					_unUsedIndexs;

			std::function<void(Type* ptr)>		_customDeletor;
		public:
			ObjectPool(size_t PoolSize = 10000);
			~ObjectPool();

			template <typename ...Args>
			std::weak_ptr<Type> make_weak(Args&&... arguments);

			template <typename ...Args>
			std::shared_ptr<Type> make_shared(Args&& ... arguments);

			template <typename ...Args>
			std::unique_ptr<Type, std::function<void(Type* ptr)>> make_unique(Args&& ... arguments);

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
		};

		template <typename Type, bool CallCtor, bool DynamicAllocateMode>
		ObjectPool<Type, CallCtor, DynamicAllocateMode>::ObjectPool(size_t PoolSize)
		{
			_customDeletor = [&](Type* ptr)
			{
				this->Free(ptr);
			};

			for(size_t index = 0; index < PoolSize; ++index)
			{
				auto object = std::malloc(sizeof(Type));
				_elementPool[object] = object;

				_unUsedIndexs.push(object);
			}
		}

		template <typename Type, bool CallCtor, bool DynamicAllocateMode>
		ObjectPool<Type, CallCtor, DynamicAllocateMode>::~ObjectPool()
		{
			for(auto object : _elementPool)
			{
				std::free(object.first);
			}

			_elementPool.clear();
		}

		template <typename Type, bool CallCtor, bool DynamicAllocateMode>
		template <typename ... Args>
		std::weak_ptr<Type> ObjectPool<Type, CallCtor, DynamicAllocateMode>::make_weak(Args&&... arguments)
		{
			auto rawPtr = Alloc(arguments...);

			return std::weak_ptr<Type>(rawPtr, _customDeletor);
		}

		template <typename Type, bool CallCtor, bool DynamicAllocateMode>
		template <typename ... Args>
		std::shared_ptr<Type> ObjectPool<Type, CallCtor, DynamicAllocateMode>::make_shared(Args&& ... arguments)
		{
			auto rawPtr = Alloc(arguments...);

			return std::shared_ptr<Type>(rawPtr, _customDeletor);
		}

		template <typename Type, bool CallCtor, bool DynamicAllocateMode>
		template <typename ... Args>
		std::unique_ptr<Type, std::function<void(Type* ptr)>> ObjectPool<Type, CallCtor, DynamicAllocateMode>::make_unique(Args&& ... arguments)
		{
			auto rawPtr = Alloc(arguments...);

			return std::unique_ptr<Type, std::function<void(Type * ptr)>>(rawPtr, _customDeletor);
		}

		template <typename Type, bool CallCtor, bool DynamicAllocateMode>
		template <typename ... Args>
		Type* ObjectPool<Type, CallCtor, DynamicAllocateMode>::Alloc(Args&&... arguments)
		{
			if(_unUsedIndexs.size() <= 0)
			{
				return nullptr; // OR Realloc
			}

			void* ptr = nullptr;

			{
				std::lock_guard<std::mutex> lock(_mutex);

				ptr = _unUsedIndexs.top();
				_unUsedIndexs.pop();
			}

			if(CallCtor)
				new (ptr) Type(arguments...);

			return reinterpret_cast<Type *>(ptr);
		}

		template <typename Type, bool CallCtor, bool DynamicAllocateMode>
		void ObjectPool<Type, CallCtor, DynamicAllocateMode>::Free(Type* ptr)
		{
			{
				std::lock_guard<std::mutex> lock(_mutex);

				auto iter = _elementPool.find(ptr);
				if (iter == _elementPool.end())
					throw;

				_unUsedIndexs.push(ptr);
			}

			if (CallCtor)
			{
				ptr->~Type();
				memset(ptr, 0, sizeof(Type)); // TEST
			}
		}
	}
}
