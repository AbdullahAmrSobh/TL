#pragma once

namespace TL
{
    template<typename Type>
    class Handle
    {
    public:
    };

    template<typename Type>
    class ResourcePool
    {
    public:
        using HandleType = Handle<Type>;

        template<typename T>
        T* operator [] (HandleType handle);

        template<typename T>
        const T* operator [] (HandleType handle) const;

        void Ref(HandleType type);

        void Release(HandleType type);

    };
} // namespace TL