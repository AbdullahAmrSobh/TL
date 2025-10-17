#pragma once

#include <vector>

#include "TL/Containers/STLAdapter.hpp"

namespace TL
{
    template<typename T>
    using Vector = std::vector<T, StlAllocatorAdapter<T>>;

    template<typename T, std::size_t Capacity = 16, bool AllowExceed = false>
    using SmallVector = std::vector<T, StlAllocatorAdapter<T>>; // Custom implementation may be needed for small storage.
} // namespace TL