#pragma once

#include <stack>
#include "Address.h"

namespace Juse
{
    template <typename T, typename U>
    inline Pair<T, U> pair(T v1, U v2) { return std::make_pair(v1, v2); }

    using Stack = std::stack<U8>;

    template <typename T>
    using Collection = Vector<Ref<T>>;

    template <typename T, typename U>
    using RefHeap = Map<T, Ref<U>>;

    template <typename T, typename U>
    using HeapMap = Map<T, SPtr<U>>;
}
