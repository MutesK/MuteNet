#pragma once

#include "foundation.h"

#ifdef _WIN32
using uint = unsigned int;
#endif

#ifdef __linux__

#define _aligned_malloc(SIZE, ALIGNMENT) aligned_malloc((ALIGNMENT), (SIZE))

#define _aligned_free aligned_free

inline bool CompareAndSwap(volatile uint* Src, uint cmp, uint& swap)
{
	return  __sync_val_compare_and_swap(Src, cmp, swap);
}

inline bool CompareAndSwap(volatile uint64_t* Src, uint64_t cmp, uint64_t& swap)
{
	return  __sync_val_compare_and_swap(Src, cmp, swap);
}

inline bool CompareAndSwap128((volatile uint64_t* pSrc, uint64_t ExchangeHigh, uint64_t ExchangeLow, uint64_t* swap)
{
	bool result;

	__asm__ __volatile__
	(
		"lock cmpxchg16b %1\n\t"
		"setz %0"
		:"=q" (result)
		, "+m" (*pSrc)
		, "+d" (ExchangeHigh)
		, "+a" (ExchangeLow)
		: "c" (swap)
		, "b" (swap >> 64)
		: "cc");

	return result;
}

inline void* aligned_malloc(size_t alignment, size_t size)
{
	return std::aligned_malloc(alignment, size);
}
#endif

#ifdef _WIN32

inline bool CompareAndSwap(volatile uint64_t* pSrc, uint64_t Cmp, uint64_t& Swap)
{
	return InterlockedCompareExchange64(reinterpret_cast<volatile LONG64 *>(pSrc), static_cast<LONG64>(Cmp), static_cast<LONG64>(Swap));
}

inline bool CompareAndSwap(volatile uint* pSrc, uint cmp, uint& Swap)
{
	return InterlockedCompareExchange(reinterpret_cast<volatile LONG *>(pSrc), static_cast<LONG>(cmp), static_cast<LONG>(Swap));
}

inline bool CompareAndSwap128(volatile uint64_t* pSrc, uint64_t ExchangeHigh, uint64_t ExchangeLow, uint64_t* swap)
{
	return InterlockedCompareExchange128(reinterpret_cast<volatile LONG64 *>(pSrc), static_cast<LONG64>(ExchangeHigh), static_cast<LONG64>(ExchangeLow), reinterpret_cast<LONG64 *>(swap));
}
#endif