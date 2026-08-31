#pragma once

#include <vector>
#include <list>             // std::list 헤더 추가
#include <unordered_map>
#include <memory>  
#include <memory_resource>

namespace MuteNet
{
	namespace Heap
	{
		using Byte = unsigned char;

		// 페이지 크기 올림 함수 (4KB 단위)
		constexpr std::size_t AlignToPageSize(std::size_t bytes) noexcept
		{
			constexpr std::size_t pageSize = 4096; // 4KB
			return (bytes + pageSize - 1) & ~(pageSize - 1);
		}

		struct Block
		{
			Byte* data = nullptr;
			std::size_t size = 0;
		};
		using RawBlockPtr = Block*;

		class MemoryResource final : public std::pmr::memory_resource
		{
		private:
			// 특정 크기별 블록 풀
			struct PagePool
			{
				std::size_t PageSize = 0;
				std::vector<RawBlockPtr> AllBlocks;
				std::list<RawBlockPtr> FreeList;
				std::unordered_map<void*, RawBlockPtr> AllocatedMap;

				~PagePool();
				void* Alloc(std::size_t alignment);
				bool Free(void* ptr);
			};

			std::unordered_map<std::size_t, PagePool> Pools;

		protected:
			void* do_allocate(std::size_t bytes, std::size_t alignment) override;
			void do_deallocate(void* ptr, std::size_t bytes, std::size_t alignment) override;
			bool do_is_equal(const std::pmr::memory_resource& other) const noexcept override;
		};
	}
}