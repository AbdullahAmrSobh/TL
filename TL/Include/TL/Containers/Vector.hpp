#pragma once

#include <vector>

#include "TL/Containers/STLAdapter.hpp"
#include "TL/Containers/InlineVector.hpp"

namespace TL
{
    template<typename T>
    using Vector = std::vector<T, StlAllocatorAdapter<T>>;
} // namespace TL