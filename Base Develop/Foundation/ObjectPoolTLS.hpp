
#include "ObjectPoolTLS.h"

template<class DATA>
atomic<uint32_t> CObjectPoolTLS<DATA>::m_lAllocCount = 0;