#pragma once

#include "TL/Containers/STLAdapter.hpp"

#include <unordered_map>

namespace TL
{
    template<typename Key, typename Value, typename Hasher = std::hash<Key>, typename KeyEq = std::equal_to<Key>>
    using Map = std::unordered_map<Key, Value, Hasher, KeyEq, StlAllocatorAdapter<std::pair<const Key, Value>>>;
} // namespace TL