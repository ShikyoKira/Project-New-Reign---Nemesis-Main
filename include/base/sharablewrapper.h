#pragma once

#include <memory>

namespace nemesis
{
    template<typename T>
    struct SharableWrapper : public std::enable_shared_from_this<nemesis::SharableWrapper<T>>
    {
    private:
        T data;

    public:
        SharableWrapper(T data)
        {
            this->data = data;
        }

        T& Get()
        {
            return data;
        }

        const T& Get() const
        {
            return data;
        }
    };
}
