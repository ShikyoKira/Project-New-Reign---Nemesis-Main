#pragma once

#include "Havok/Base/hkpConstraintMotor.h"

#include "Havok/hkpGenericConstraintDataSchemeConstraintInfo.h"

namespace nemesis
{
    struct hkpGenericConstraintDataScheme : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0x11fd6f6c,
                                                "hkpGenericConstraintDataScheme",
                                                nullptr,
                                                80,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpGenericConstraintDataScheme);

        nemesis::hkpGenericConstraintDataSchemeConstraintInfo m_info;
        nemesis::hkArray<nemesis::hkVector4> m_data;
        nemesis::hkArray<int> m_commands;
        nemesis::hkArray<nemesis::hkRefPtr<nemesis::HavokObject>> m_modifiers;
        nemesis::hkArray<nemesis::hkRefPtr<nemesis::hkpConstraintMotor>> m_motors;

    public:
        hkpGenericConstraintDataScheme() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpGenericConstraintDataScheme::Class;
}
