#pragma once

#include "Havok/Base/hkVariant.h"

#include "Serialize/Deserializer.h"

namespace nemesis
{
    struct Deserializer;

    struct hkRefVariant : nemesis::hkVariant, nemesis::HavokObject
    {
        hkRefVariant(unsigned int signature)
            : nemesis::HavokObject(signature)
        {
        }

        virtual bool IsFixed() const = 0;
        virtual bool IsNull() const  = 0;
        virtual void Clear()         = 0;

        virtual nemesis::HavokObject* ReferenceTo(nemesis::HavokObject* ref_obj) = 0;

        virtual nemesis::HavokObject& GetReference()             = 0;
        virtual const nemesis::HavokObject& GetReference() const = 0;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override = 0;

        void SerializeTo(nemesis::Serializer& serializer) const override   = 0;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override = 0;
    };

    template <typename T, bool Fixed, bool Proxy>
    struct hkRefPtr : nemesis::hkRefVariant
    {
    private:
        T* Pointer = nullptr;

    public:
        hkRefPtr()
            : nemesis::hkRefVariant(0x1e3857bb)
        {
            static_assert(std::is_base_of_v<nemesis::HavokObject, T>
                              || std::is_same_v<nemesis::HavokObject, T>,
                          "T must be HavokObject or derived from HavokObject");

            if constexpr (Fixed)
            {
                Pointer = new T();
            }
        }

        ~hkRefPtr() override
        {
            if constexpr (!Fixed || Proxy) return;

            delete Pointer;
        }

        bool IsFixed() const override
        {
            return Fixed;
        }

        bool IsNull() const override
        {
            return Pointer == nullptr;
        }

        void Clear() override
        {
            if constexpr (Fixed)
            {
                throw std::runtime_error("Fixed hkRefPtr cannot be cleared");
            }

            Pointer = nullptr;
        }

        nemesis::HavokObject* ReferenceTo(nemesis::HavokObject* ref_obj) override
        {
            if constexpr (Fixed)
            {
                throw std::runtime_error("Fixed hkRefPtr cannot change reference");
            }

            Pointer = dynamic_cast<T*>(ref_obj);
            return Pointer;
        }
        
        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override
        {
            if (Pointer == nullptr) return nullptr;

            return Pointer->GetClass(version);
        }

        nemesis::HavokObject& GetReference() override
        {
            return *Pointer;
        }

        const nemesis::HavokObject& GetReference() const override
        {
            return *Pointer;
        }

        T& GetReferencedObject()
        {
            return *Pointer;
        }

        const T& GetReferencedObject() const
        {
            return *Pointer;
        }

        void SerializeTo(nemesis::Serializer& serializer) const override
        {
            if (IsNull()) return;

            Pointer->SerializeTo(serializer);
        }

        void DeserializeFrom(nemesis::Deserializer& deserializer) override
        {
            if (Pointer == nullptr) return;

            Pointer->DeserializeFrom(deserializer);
        }
    };
}
