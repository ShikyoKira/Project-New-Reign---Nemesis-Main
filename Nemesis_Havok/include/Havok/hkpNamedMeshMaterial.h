#pragma once

#include "Havok/hkpMeshMaterial.h"

namespace nemesis
{
    struct hkpNamedMeshMaterial : nemesis::hkpMeshMaterial
    {
        static constexpr nemesis::hkClass Class{0x66b42df1,
                                                "hkpNamedMeshMaterial",
                                                &nemesis::hkpMeshMaterial::Class,
                                                16,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpNamedMeshMaterial);

        nemesis::hkStringPtr m_name;

    public:
        hkpNamedMeshMaterial() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpNamedMeshMaterial::Class;
}
