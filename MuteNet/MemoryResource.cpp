#include "MemoryResource.h"
#include <new>

namespace MuteNet
{
	namespace Heap
	{
		MemoryResource::PagePool::~PagePool()
		{
			for (auto* block : AllBlocks)
			{
				// delete[] block->data; 대신 _aligned_free 사용
				if (block->data)
				{
					_aligned_free(block->data);
				}
				delete block;
			}
		}

		void* MemoryResource::PagePool::Alloc(std::size_t alignment)
		{
			// 최소 64바이트(캐시라인/AVX512 안전선) 이상으로 alignment 보장
			const std::size_t requiredAlign = std::max<std::size_t>(alignment, 64);

			if (FreeList.empty())
			{
				// 새로운 블록 할당 시 _aligned_malloc 사용
				auto* newBlock = new Block;
				newBlock->size = PageSize;

				// 64바이트 이상으로 정렬된 주소 할당
				newBlock->data = static_cast<Byte*>(_aligned_malloc(PageSize, requiredAlign));
				if (!newBlock->data)
				{
					delete newBlock;
					throw std::bad_alloc();
				}

				AllBlocks.push_back(newBlock);
				FreeList.push_back(newBlock);

				AllocatedMap[newBlock->data] = newBlock;
			}

			auto* block = FreeList.front();
			FreeList.pop_front();
			AllocatedMap[block->data] = block;
			return block->data;
		}

		bool MemoryResource::PagePool::Free(void* ptr)
		{
			auto iter = AllocatedMap.find(ptr);
			if (iter == AllocatedMap.end())
			{
				return false;
			}

			RawBlockPtr block = iter->second;
			AllocatedMap.erase(iter);
			FreeList.push_back(block);
			return true;
		}

		void* MemoryResource::do_allocate(std::size_t bytes, std::size_t alignment)
		{
			if (bytes == 0)
			{
				return nullptr;
			}

			const size_t alignedSize = AlignToPageSize(bytes);

			// operator[]를 사용하여 없으면 자동 생성 후 초기화 (재귀 제거)
			auto& pool = Pools[alignedSize];
			if (pool.PageSize == 0)
			{
				pool.PageSize = alignedSize;
			}

			return pool.Alloc(alignment);
		}

		void MemoryResource::do_deallocate(void* ptr, std::size_t bytes, std::size_t alignment)
		{
			if (ptr == nullptr)
				return;

			const size_t alignedSize = AlignToPageSize(bytes);
			auto it = Pools.find(alignedSize);

			if (it != Pools.end())
			{
				it->second.Free(ptr);
			}
		}

		bool MemoryResource::do_is_equal(const std::pmr::memory_resource& other) const noexcept
		{
			return this == &other;
		}
	}
}