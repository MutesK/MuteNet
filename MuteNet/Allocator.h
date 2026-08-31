#pragma once

#include "MemoryResource.h"
#include <utility>
#include <new>

namespace MuteNet
{
	class Allocator final
	{
	public:
		Allocator() = delete;
		~Allocator() = delete;
		Allocator(const Allocator&) = delete;
		Allocator& operator=(const Allocator&) = delete;

		static Heap::MemoryResource& GetResource()
		{
			static Heap::MemoryResource resource;
			return resource;
		}

		template <typename T, typename... Args>
		[[nodiscard]] static T* New(Args&&... args)
		{
			auto& res = GetResource();
			void* ptr = res.allocate(sizeof(T), alignof(T));
			if (!ptr)
			{
				throw std::bad_alloc();
			}

			try
			{
				// Correct Placement New Syntax
				return ::new (ptr) T(std::forward<Args>(args)...);
			}
			catch (...)
			{
				res.deallocate(ptr, sizeof(T), alignof(T));
				throw;
			}
		}

		template <typename T>
		static void Delete(T* ptr) noexcept
		{
			if (ptr)
			{
				ptr->~T();
				auto& res = GetResource();
				res.deallocate(static_cast<void*>(ptr), sizeof(T), alignof(T));
			}
		}
	};
}