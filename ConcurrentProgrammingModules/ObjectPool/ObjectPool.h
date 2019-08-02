#pragma once

#include "../ConcurrentProgrammingModules/CommonHeader.h"

namespace Util
{
	namespace TL
	{
		template <class Type>
		class ObjectPool
		{
		private:
			struct st_BLOCK_NODE
			{
				Type type;
				st_BLOCK_NODE* nextBlock;
			};

			struct st_Pop
			{
				st_BLOCK_NODE* topNode;
				__int64		   uniqueCount;
			};
		public:

			ObjectPool(int blockSize = 20000);
			virtual ~ObjectPool();

			Type* Alloc(void);
			bool Free(Type* pType);


			size_t GetBlockCount(void) const;
			size_t GetAllocCount(void) const;

			Type* operator()();

			NON_COPYABLE(ObjectPool);
		private:
			void MakeNewNode();
		private:
			// 생성시 할당량
			volatile LONG BlockSize = 0;

			volatile LONG AllocCount = 0;
			volatile LONG64 Unique = 0;

			st_Pop* pPop;
			volatile st_BLOCK_NODE* pTail;

			std::list<st_BLOCK_NODE*> List;
		};
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		template <class Type>
		ObjectPool<Type>::ObjectPool(int blockSize)
		{
			st_BLOCK_NODE* pNewNode = nullptr;
			st_BLOCK_NODE* pOldNode = nullptr;

			pNewNode = static_cast<st_BLOCK_NODE*>(malloc(sizeof(st_BLOCK_NODE)));
			memset(pNewNode, 0, sizeof(st_BLOCK_NODE));
			memset(&pNewNode->Type, 0, sizeof(Type));
			List.push_back(pNewNode);

			pPop = static_cast<st_Pop*>(_aligned_malloc(128, 16));
			pPop->_TopNode = pNewNode;
			pPop->UniqueCount = 0;
			pOldNode = pNewNode;


			for (auto i = 1; i <= blockSize; i++)
			{
				pNewNode = static_cast<st_BLOCK_NODE*>(malloc(sizeof(st_BLOCK_NODE)));
				pOldNode->pNextBlock = pNewNode;
				pNewNode->pNextBlock = nullptr;
				memset(&pNewNode->Type, 0, sizeof(Type));

				List.push_back(pNewNode);

				pOldNode = pNewNode;
				BlockSize++;
			}

			pTail = pNewNode;
		}

		template <class Type>
		ObjectPool<Type>::~ObjectPool()
		{
			auto iter = List.begin();

			for (; iter != List.end();)
			{
				st_BLOCK_NODE* pNode = (*iter);
				List.erase(iter);
				free(pNode);
			}

			_aligned_free(pPop);

		}

		template <class Type>
		Type* ObjectPool<Type>::Alloc(void)
		{
			st_Pop OldPop;
			st_BLOCK_NODE* pNewNode = nullptr;
			Type* ret = nullptr;
			const size_t UniqueCount = InterlockedIncrement64(&Unique) % MAXLONG64;

		RETRY:
			if (pPop->_TopNode->pNextBlock == nullptr)
			{
				for (auto i = 0; i < 10000; i++)
					MakeNewNode();
			}

			while (true)
			{
				OldPop = { 0 };

				// 자료구조를 변경한다.
				OldPop._TopNode = pPop->_TopNode;
				OldPop.UniqueCount = pPop->UniqueCount;

				if (OldPop._TopNode->pNextBlock == nullptr)
					goto RETRY;

				pNewNode = OldPop._TopNode->pNextBlock;

				if (InterlockedCompareExchange128(static_cast<LONG64*>(pPop),
					static_cast<size_t>(UniqueCount), static_cast<size_t>(pNewNode),
					static_cast<LONG64*>(&OldPop)))
				{
					ret = &OldPop._TopNode->Type;
					break;
				}

			}
			InterlockedIncrement(&AllocCount);

			return ret;
		}

		template <class Type>
		bool ObjectPool<Type>::Free(Type* pType)
		{
			auto pDel = static_cast<st_BLOCK_NODE*>(pType);

			if (InterlockedAdd(&AllocCount, -1) < 0)
			{
				InterlockedAdd(&AllocCount, 1);
				return false;
			}

			st_Pop OldPop;
			LONG64 UniqueCount = InterlockedIncrement64(&Unique) % MAXLONG64;
			while (true)
			{
				OldPop = { 0 };

				OldPop._TopNode = pPop->_TopNode;
				OldPop.UniqueCount = pPop->UniqueCount;

				if (OldPop._TopNode != pDel->pNextBlock && OldPop._TopNode != pDel)
					pDel->pNextBlock = OldPop._TopNode;

				if (InterlockedCompareExchange128(static_cast<LONG64*>(pPop),
					static_cast<size_t>(UniqueCount), static_cast<size_t>(pDel),
					static_cast<LONG64*>(&OldPop)))
					break;
			}

			return true;
		}


		template <class Type>
		void ObjectPool<Type>::MakeNewNode()
		{
			st_BLOCK_NODE* pNewNode = static_cast<st_BLOCK_NODE*>(malloc(sizeof(st_BLOCK_NODE)));
			memset(pNewNode, 0, sizeof(st_BLOCK_NODE));
			pNewNode->pNextBlock = nullptr;

			List.push_back(pNewNode);

			st_Pop OldPop;
			const size_t UniqueCount = InterlockedIncrement64(&Unique) % MAXLONG64;

			while (true)
			{
				OldPop = { 0 };

				OldPop._TopNode = pPop->_TopNode;
				OldPop.UniqueCount = pPop->UniqueCount;

				if (OldPop._TopNode != pNewNode->pNextBlock && OldPop._TopNode != pNewNode)
					pNewNode->pNextBlock = OldPop._TopNode;
				else continue;

				if (InterlockedCompareExchange128(static_cast<LONG64*>(pPop), static_cast<size_t>(UniqueCount),
					static_cast<size_t>(pNewNode), static_cast<LONG64*>(&OldPop)))
				{
					InterlockedAdd(&BlockSize, 1);
					break;
				}
			}
		}

		template <class Type>
		size_t ObjectPool<Type>::GetBlockCount(void) const
		{
			return BlockSize;
		}

		template <class Type>
		size_t ObjectPool<Type>::GetAllocCount(void) const
		{
			return AllocCount;
		}

		template <class Type>
		Type* ObjectPool<Type>::operator()()
		{
			return Alloc();
		}
	}
}