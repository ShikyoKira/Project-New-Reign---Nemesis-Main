#pragma once

#include "utilities/types.h"

#include "base/havokdata.h"

namespace nemesis
{
    template <typename T>
    struct HavokDataList
    {
    private:
        Vec<T> list;

    public:
        HavokDataList() = default;

        void Add(const std::string& name, size_t id)
        {
            list.emplace_back(T(name, id));
        }

        size_t Size() const noexcept
        {
            return list.size();
        }

    protected:
        // return null if doesn't exist
        const T* GetByName(const std::string& name) const noexcept
        {
            for (auto& each : list)
            {
                if (each.GetName() == name) return &each;
            }

            return nullptr;
        }

        // return null if doesn't exist
        const T* GetById(size_t id) const noexcept
        {
            for (auto& each : list)
            {
                if (each.GetId() == id) return &each;
            }

            return nullptr;
        }

        bool HasName(const std::string& name) const noexcept
        {
            return GetByName(name) != nullptr;
        }

        bool HasId(size_t id) const noexcept
        {
            return GetById(id) != nullptr;
        }
    };
} // namespace nemesis
