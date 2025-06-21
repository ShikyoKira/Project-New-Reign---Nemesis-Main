#pragma once

#include "Havok/hkxAttribute.h"

namespace nemesis
{
    struct hkxAttributeGroup : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0x345ca95d,
                                                "hkxAttributeGroup",
                                                nullptr,
                                                24,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkxAttributeGroup);

        nemesis::hkStringPtr m_name;
        nemesis::hkArray<nemesis::hkxAttribute> m_attributes;

    public:
        hkxAttributeGroup() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkxAttributeGroup::Class;
}
