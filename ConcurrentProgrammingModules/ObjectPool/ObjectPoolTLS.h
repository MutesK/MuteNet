#pragma once

#include "ObjectPool.h"

namespace Util
{
	template <class Type>
	class ObjectPoolTLS
	{
	public:
		class CChunkBlock;
		struct st_ChunkType
		{
			Type	Type;
			CChunkBlock* pThisChunk;
			ObjectPool<CChunkBlock>* pObjectPool;
		};

		class CChunkBlock
		{
		public:
			CChunkBlock(ObjectPool<CChunkBlock>* pObjectPool, int BlockSize = 2000)
			{
				if (pArrayChunk == nullptr)
					pArrayChunk = static_cast<st_ChunkType*>(malloc(sizeof(st_ChunkType) * BlockSize));

				for (int i = 0; i < BlockSize; i++)
				{
					pArrayChunk[i].pThisChunk = this;
					pArrayChunk[i].pObjectPool = pObjectPool;
				}

				AllocCount = 0;
				ReferenceCount = BlockSize;
			}

			Type* Alloc()
			{
				Type* ret = &pArrayChunk[AllocCount].Type;
				++AllocCount;

				return ret;
			}
			bool Free(Type* pType, st_ChunkType* pBlock)
			{
				if (--ReferenceCount == 0)
				{
					pBlock->pObjectPool->Free(pBlock->pThisChunk);
				}

				--AllocCount;

				return true;
			}

			friend class ObjectPoolTLS;
		private:
			volatile st_ChunkType* pArrayChunk;

			std::atomic<size_t>	ReferenceCount;
			std::atomic<size_t>	AllocCount;
		};

	public:
		ObjectPoolTLS(int ChunkSize = 1000, int BlockSize = 0)
			:BlockSize(BlockSize), ChunkSize(ChunkSize)
		{
			AllocCount = 0;

			pObjectPool = new ObjectPool<CChunkBlock>(BlockSize);

			TLSIndex = TlsAlloc();
			if (TLSIndex == TLS_OUT_OF_INDEXES)
			{
				int* p = nullptr;
				*p = 0;
			}
		}

		~ObjectPoolTLS() = default;

		template<class... Args >
		std::shared_ptr<Type> get_shared(Args&& ... args)
		{
			Type* ptr = Alloc();
			std::shared_ptr<Type> pRet(ptr, [&](Type* ptr)
				{
					this->Free(ptr);
				});
			new (*pRet) Type(args);

			return pRet;
		}

		std::shared_ptr<Type> get_shared()
		{
			Type* ptr = Alloc();
			std::shared_ptr<Type> pRet(ptr, [&](Type* ptr)
				{
					this->Free(ptr);
				});
			new (pRet.get()) Type();

			return pRet;
		}

		template <class... Args>
		std::unique_ptr<Type> get_unique(Args&& ... args)
		{
			Type* ptr = Alloc();
			std::unique_ptr<Type> pRet(ptr, [&](Type* ptr)
				{
					this->Free(ptr);
				});
			new (*pRet) Type(args);

			return pRet;
		}

		std::unique_ptr<Type> get_unique()
		{
			Type* ptr = Alloc();
			std::unique_ptr<Type> pRet(ptr, [&](Type* ptr)
				{
					this->Free(ptr);
				});
			new (*pRet) Type();

			return pRet;
		}

		template <class... Args>
		std::weak_ptr<Type> get_weak(Args&& ... args)
		{
			Type* ptr = Alloc();
			std::weak_ptr<Type> pRet(ptr, [&](Type* ptr)
				{
					this->Free(ptr);
				});
			new (*pRet) Type(args);

			return pRet;
		}

		std::weak_ptr<Type> get_weak()
		{
			Type* ptr = Alloc();
			std::weak_ptr<Type> pRet(ptr, [&](Type* ptr)
				{
					this->Free(ptr);
				});
			new (*pRet) Type();

			return pRet;
		}

		size_t GetChunkSize()
		{
			return pObjectPool->GetAllocCount();
		}
		size_t GetAllocCount() const
		{
			return AllocCount;
		}

		Type* Alloc()
		{
			auto pBlock = static_cast<CChunkBlock*>(TlsGetValue(TLSIndex));

			if (pBlock == nullptr)
			{
				pBlock = pObjectPool->Alloc();
				new (pBlock) CChunkBlock(pObjectPool, ChunkSize);
				TlsSetValue(TLSIndex, pBlock);
			}

			Type* pRet = pBlock->Alloc();
			++AllocCount;


			if (pBlock->m_lAllocCount == ChunkSize || pBlock->m_lReferenceCount == 0)
				TlsSetValue(TLSIndex, nullptr);

			return pRet;
		}
		void Free(Type* pType)
		{
			auto pBlock = reinterpret_cast<st_ChunkType*>(pType);

			if (pBlock->pThisChunk->Free(pType, pBlock))
				--AllocCount;

			pType->~Type();
		}
	public:
		std::atomic<size_t>		AllocCount;
		std::atomic<size_t>		BlockSize;
		std::atomic<size_t>		ChunkSize;
	private:
		volatile ObjectPool<CChunkBlock>* pObjectPool;

		uint32_t TLSIndex;
	};
}