#pragma once

#include "TL/Containers/STLAdapter.hpp"

#include <unordered_set>

namespace TL
{
    template<typename Key, typename Hasher = std::hash<Key>, typename KeyEq = std::equal_to<Key>>
    using Set = std::unordered_set<Key, Hasher, KeyEq, StlAllocatorAdapter<Key>>;
} // namespace TL