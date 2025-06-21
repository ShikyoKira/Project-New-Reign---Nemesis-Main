#pragma once

#include "Havok/Base/HavokObject.h"

#include "Serialize/Deserializer.h"
#include "Serialize/Serializer.h"

namespace nemesis
{
    struct hkArrayBase : nemesis::HavokObject
    {
    public:
        hkArrayBase(unsigned int signature)
            : nemesis::HavokObject(signature)
        {
        }

        virtual unsigned int GetSize() const    = 0;
        virtual void SetSize(unsigned int size) = 0;

        virtual bool IsCString() const = 0;
        virtual bool IsVariant() const = 0;
        virtual bool IsEmpty() const   = 0;
        virtual void Clear()           = 0;

        void SerializeTo(nemesis::Serializer& serializer) const override   = 0;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override = 0;
    };

    struct hkSmallArrayBase : nemesis::HavokObject
    {
    public:
        hkSmallArrayBase(unsigned int signature)
            : nemesis::HavokObject(signature)
        {
        }

        virtual unsigned int GetSize() const    = 0;
        virtual void SetSize(unsigned int size) = 0;

        virtual bool IsCString() const = 0;
        virtual bool IsVariant() const = 0;
        virtual bool IsEmpty() const   = 0;
        virtual void Clear()           = 0;

        void SerializeTo(nemesis::Serializer& serializer) const override   = 0;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override = 0;

    };

    template<typename T>
    struct hkArrayCommon
    {
    private:
        using UnpointedType = std::remove_pointer_t<T>;

        Vec<UPtr<T>> List;

    public:
        hkArrayCommon()
        {
            static_assert(std::is_base_of_v<nemesis::HavokObject, UnpointedType>
                              || std::is_same_v<nemesis::HavokObject, UnpointedType>
                              || std::is_base_of_v<nemesis::hkVariant, UnpointedType>
                              || std::is_arithmetic_v<UnpointedType>,
                          "T must be either number type, HavokObject type, derived from HavokObject or "
                          "derived from hkVariant");
        }

        virtual ~hkArrayCommon() {}

        Vec<UPtr<T>>::iterator begin() noexcept
        {
            return List.begin();
        }

        Vec<UPtr<T>>::iterator end() noexcept
        {
            return List.end();
        }
        
        Vec<UPtr<T>>::const_iterator begin() const noexcept
        {
            return List.begin();
        }

        Vec<UPtr<T>>::const_iterator end() const noexcept
        {
            return List.end();
        }

        virtual unsigned int GetSize() const
        {
            return static_cast<unsigned int>(List.size());
        }
        
        virtual void SetSize(unsigned int size)
        {
            List.clear();
            List.reserve(size);

            for (unsigned int i = 0; i < size; ++i)
            {
                if constexpr (std::is_default_constructible_v<T>
                              && !std::is_abstract_v<T>)
                {
                    List.emplace_back(new T());
                }
                else
                {
                    List.emplace_back();
                }
            }
        }

        virtual bool IsCString() const
        {
            if constexpr (!std::is_pointer_v<T> && std::is_same_v<nemesis::hkCString, T>)
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        virtual bool IsVariant() const
        {
            if constexpr (!std::is_pointer_v<T> && std::is_base_of_v<nemesis::HavokObject, T>
                          && !std::is_base_of_v<nemesis::hkRefVariant, T>)
            {
                return false;
            }
            else
            {
                return true;
            }
        }

        virtual bool IsEmpty() const
        {
            return List.empty();
        }

        virtual void Clear()
        {
            List.clear();
        }

        T& Back()
        {
            return *List.back();
        }

        const T& Back() const
        {
            return *List.back();
        }

        T& Front()
        {
            return *List.front();
        }

        const T& Front() const
        {
            return *List.front();
        }

        template <typename... ValTy>
        T& PushBack(ValTy&&... args)
        {
            return *List.emplace_back(std::forward<ValTy>(args)...);
        }

        void PopBack()
        {
            List.pop_back();
        }

        T& operator[](int index)
        {
            return *List[index];
        }

        const T& operator[](int index) const
        {
            return *List[index];
        }

        void RemoveAt(int index)
        {
            List.erase(List.begin() + index);
        }

        virtual void SerializeTo(nemesis::Serializer& serializer) const
        {
            for (auto& each : List)
            {
                if constexpr (std::is_pointer_v<T> || std::is_base_of_v<nemesis::HavokObject, T>)
                {
                    serializer.WriteObject("", *each);
                }
                else if constexpr (std::is_same_v<nemesis::hkCString, T>)
                {
                    serializer.WriteValue("", *each, 2);
                }
                else
                {
                    serializer.WriteValue("", *each);
                }
            }
        }

        virtual void DeserializeFrom(nemesis::Deserializer& deserializer)
        {
            if constexpr (!std::is_default_constructible_v<T> || std::is_abstract_v<T>)
            {
                return;
            }

            for (auto& each : List)
            {
                if constexpr (std::is_pointer_v<T> || std::is_base_of_v<nemesis::HavokObject, T>)
                {
                    deserializer.ReadObject("", *each);
                }
                else if constexpr (std::is_same_v<nemesis::hkCString, T>)
                {
                    deserializer.ReadValue("", *each, 2);
                }
                else
                {
                    deserializer.ReadValue("", *each);
                }
            }
        }
    };

    template<typename T>
    struct hkSmallArray : nemesis::hkArrayCommon<T>, nemesis::hkSmallArrayBase
    {
    public:
        hkSmallArray()
            : nemesis::hkSmallArrayBase(0xee3c2aec)
        {
        }

        unsigned int GetSize() const override
        {
            return nemesis::hkArrayCommon<T>::GetSize();
        }

        void SetSize(unsigned int size) override
        {
            nemesis::hkArrayCommon<T>::SetSize(size);
        }

        bool IsCString() const override
        {
            return nemesis::hkArrayCommon<T>::IsCString();
        }

        bool IsVariant() const override
        {
            return nemesis::hkArrayCommon<T>::IsVariant();
        }

        bool IsEmpty() const override
        {
            return nemesis::hkArrayCommon<T>::IsEmpty();
        }

        void Clear() override
        {
            nemesis::hkArrayCommon<T>::Clear();
        }

        void SerializeTo(nemesis::Serializer& serializer) const override
        {
            nemesis::hkArrayCommon<T>::SerializeTo(serializer);
        }

        void DeserializeFrom(nemesis::Deserializer& deserializer) override
        {
            nemesis::hkArrayCommon<T>::DeserializeFrom(deserializer);
        }
    };

    template<typename T>
    struct hkArray : nemesis::hkArrayCommon<T>, nemesis::hkArrayBase
    {
    public:
        hkArray()
            : nemesis::hkArrayBase(0xd404a39a)
        {
        }

        unsigned int GetSize() const override
        {
            return nemesis::hkArrayCommon<T>::GetSize();
        }

        void SetSize(unsigned int size) override
        {
            nemesis::hkArrayCommon<T>::SetSize(size);
        }

        bool IsCString() const override
        {
            return nemesis::hkArrayCommon<T>::IsCString();
        }

        bool IsVariant() const override
        {
            return nemesis::hkArrayCommon<T>::IsVariant();
        }

        bool IsEmpty() const override
        {
            return nemesis::hkArrayCommon<T>::IsEmpty();
        }

        void Clear() override
        {
            nemesis::hkArrayCommon<T>::Clear();
        }

        void SerializeTo(nemesis::Serializer& serializer) const override
        {
            nemesis::hkArrayCommon<T>::SerializeTo(serializer);
        }

        void DeserializeFrom(nemesis::Deserializer& deserializer) override
        {
            nemesis::hkArrayCommon<T>::DeserializeFrom(deserializer);
        }
    };
}
