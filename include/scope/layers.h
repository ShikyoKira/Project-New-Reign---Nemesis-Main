#pragma once

#include "utilities/types.h"

namespace nemesis
{
    struct ILayers
    {
        virtual void DropLayer() noexcept = 0;
    };

    template <typename T>
    struct Layers : public ILayers
    {
    private:
        // persist until scope ends, then drop a layer (open and close = 1 scope)
        Vec<T> layers;

        // persist only throughout the processing session (between '$' and '$')
        WPtr<T> temp;

    public:
        Layers() = default;

        const T& GetValue() const
        {
            if (temp.expired()) return layers.back();

            return *temp.lock();
        }

        T& GetValue()
        {
            if (temp.expired()) return layers.back();

            return *temp.lock();
        }

        bool Empty() const
        {
            return temp.expired() && layers.empty();
        }

        void AddLayer(const T& value)
        {
            layers.emplace_back(value);
        }
        
        void SetTemp(SPtr<T>& temp) noexcept
        {
            this->temp = temp;
        }
        
        void UpdateLayer(T value)
        {
            layers.back() = value;
        }

        void DropLayer() noexcept
        {
            layers.pop_back();
        }
    };
} // namespace nemesis
