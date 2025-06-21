#pragma once

#include "Havok/Base/hkReferencedObject.h"

#include "Havok/hkxEnumItem.h"

namespace nemesis
{
    struct hkxEnum : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0xc4e1211,
                                                "hkxEnum",
                                                &nemesis::hkReferencedObject::Class,
                                                32,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkxEnum);

        nemesis::hkArray<nemesis::hkxEnumItem> m_items;

    public:
        hkxEnum() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkxEnum::Class;
}
