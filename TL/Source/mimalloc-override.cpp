#include <mimalloc.h>
// #include <mimalloc-new-delete.h>

#include <cstdint>

#if TL_ENABLE_TRACY
    #include <tracy/Tracy.hpp>
#endif

namespace TL
{
    inline static void OnNew([[maybe_unused]] void* ptr, [[maybe_unused]] size_t count)
    {
#if TL_ENABLE_TRACY && TL_ENABLE_TRACY_MEMORY_TRACKING
        TracyAllocS(ptr, count, 20);
#endif

#if TL_ENABLE_LEAK_REPORTER
        // TODO: track allocation
#endif
    }

    inline static void OnDelete([[maybe_unused]] void* ptr)
    {
#if TL_ENABLE_TRACY && TL_ENABLE_TRACY_MEMORY_TRACKING
        TracyFreeS(ptr, 20);
#endif
    }
} // namespace TL

// @todo: revist this
// #if defined(_MSC_VER) && defined(_Ret_notnull_) && defined(_Post_writable_byte_size_)
//   // stay consistent with VCRT definitions
//     #define mi_decl_new(n) mi_decl_nodiscard mi_decl_restrict _Ret_notnull_ _Post_writable_byte_size_(n)
//     #define mi_decl_new_nothrow(n) mi_decl_nodiscard mi_decl_restrict _Ret_maybenull_ _Success_(return != NULL) _Post_writable_byte_size_(n)
// #else
//     #define mi_decl_new(n) mi_decl_nodiscard mi_decl_restrict
//     #define mi_decl_new_nothrow(n) mi_decl_nodiscard mi_decl_restrict
// #endif

#define mi_decl_new(n)
#define mi_decl_new_nothrow(n)

void operator delete(void* p) noexcept
{
    mi_free(p);
    TL::OnDelete(p);
}

void operator delete[](void* p) noexcept
{
    mi_free(p);
    TL::OnDelete(p);
}

void operator delete(void* p, const std::nothrow_t&) noexcept
{
    mi_free(p);
    TL::OnDelete(p);
}

void operator delete[](void* p, const std::nothrow_t&) noexcept
{
    mi_free(p);
    TL::OnDelete(p);
}

mi_decl_new(n) void* operator new(std::size_t n) noexcept(false)
{
    auto ptr = mi_new(n);
    TL::OnNew(ptr, n);
    return ptr;
}

mi_decl_new(n) void* operator new[](std::size_t n) noexcept(false)
{
    auto ptr = mi_new(n);
    TL::OnNew(ptr, n);
    return ptr;
}

mi_decl_new_nothrow(n) void* operator new(std::size_t n, const std::nothrow_t& tag) noexcept
{
    (void)(tag);
    auto ptr = mi_new_nothrow(n);
    TL::OnNew(ptr, n);
    return ptr;
}

mi_decl_new_nothrow(n) void* operator new[](std::size_t n, const std::nothrow_t& tag) noexcept
{
    (void)(tag);
    auto ptr = mi_new_nothrow(n);
    TL::OnNew(ptr, n);
    return ptr;
}

#if (__cplusplus >= 201402L || _MSC_VER >= 1916)
void operator delete(void* p, std::size_t n) noexcept
{
    mi_free_size(p, n);
    TL::OnDelete(p);
};

void operator delete[](void* p, std::size_t n) noexcept
{
    mi_free_size(p, n);
    TL::OnDelete(p);
};
#endif

#if (__cplusplus > 201402L || defined(__cpp_aligned_new))

void operator delete(void* p, std::align_val_t al) noexcept
{
    mi_free_aligned(p, static_cast<size_t>(al));
    TL::OnDelete(p);
}

void operator delete[](void* p, std::align_val_t al) noexcept
{
    mi_free_aligned(p, static_cast<size_t>(al));
    TL::OnDelete(p);
}

void operator delete(void* p, std::size_t n, std::align_val_t al) noexcept
{
    mi_free_size_aligned(p, n, static_cast<size_t>(al));
    TL::OnDelete(p);
};

void operator delete[](void* p, std::size_t n, std::align_val_t al) noexcept
{
    mi_free_size_aligned(p, n, static_cast<size_t>(al));
    TL::OnDelete(p);
};

void operator delete(void* p, std::align_val_t al, const std::nothrow_t&) noexcept
{
    mi_free_aligned(p, static_cast<size_t>(al));
    TL::OnDelete(p);
}

void operator delete[](void* p, std::align_val_t al, const std::nothrow_t&) noexcept
{
    mi_free_aligned(p, static_cast<size_t>(al));
    TL::OnDelete(p);
}

void* operator new(std::size_t n, std::align_val_t al) noexcept(false)
{
    auto ptr = mi_new_aligned(n, static_cast<size_t>(al));
    TL::OnNew(ptr, n);
    return ptr;
}

void* operator new[](std::size_t n, std::align_val_t al) noexcept(false)
{
    auto ptr = mi_new_aligned(n, static_cast<size_t>(al));
    TL::OnNew(ptr, n);
    return ptr;
}

void* operator new(std::size_t n, std::align_val_t al, const std::nothrow_t&) noexcept
{
    auto ptr = mi_new_aligned_nothrow(n, static_cast<size_t>(al));
    TL::OnNew(ptr, n);
    return ptr;
}

void* operator new[](std::size_t n, std::align_val_t al, const std::nothrow_t&) noexcept
{
    auto ptr = mi_new_aligned_nothrow(n, static_cast<size_t>(al));
    TL::OnNew(ptr, n);
    return ptr;
}

#endif