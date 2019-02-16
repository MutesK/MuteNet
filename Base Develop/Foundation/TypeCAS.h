#pragma once

#include "foundation.h"

#ifdef _WIN32
using uint = unsigned int;
#endif


#pragma pack(push, 16) 
struct uint128_t
{
	uint64_t _low;
	uint64_t _high;
};
#pragma pack(pop)

#ifdef __linux__
inline bool CompareAndSwap(volatile uint* Src, uint cmp, uint swap)
{
	return  __sync_val_compare_and_swap(Src, cmp, swap);
}

inline bool CompareAndSwap(volatile uint64_t* Src, uint64_t cmp, uint64_t swap)
{
	return  __sync_val_compare_and_swap(Src, cmp, swap);
}

inline bool CompareAndSwap(volatile uint128_t* pSrc, uint128_t cmp, uint128_t swap)
{
	bool result;

	__asm__ __volatile__
	(
		"lock cmpxchg16b %1\n\t"
		"setz %0"
		:"=q" (result)
		, "+m" (*pSrc)
		, "+d" (cmp._high)
		, "+a" (cmp._low)
		: "c" (swap._high)
		, "b" (swap._low)
		: "cc");

	return result;
}
#endif

#ifdef _WIN32
#include <Windows.h>

inline bool CompareAndSwap(volatile uint64_t* pSrc, uint64_t Cmp, uint64_t Swap)
{
	return InterlockedCompareExchange64(reinterpret_cast<volatile LONG64 *>(pSrc), static_cast<LONG64>(Cmp), static_cast<LONG64>(Swap));
}

inline bool CompareAndSwap(volatile uint* pSrc, uint cmp, uint Swap)
{
	return InterlockedCompareExchange(reinterpret_cast<volatile LONG *>(pSrc), static_cast<LONG>(cmp), static_cast<LONG>(Swap));
}

inline bool CompareAndSwap128(volatile uint128_t* pSrc, uint128_t cmp, uint128_t swap)
{
	return InterlockedCompareExchange128(reinterpret_cast<volatile LONG64 *>(pSrc), static_cast<LONG64>(cmp._high), static_cast<LONG64>(cmp._low), reinterpret_cast<LONG64 *>(&swap));
}
#endif