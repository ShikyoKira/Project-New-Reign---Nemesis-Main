#pragma once

#include "Havok/hkClass.h"

#include "Serialize/Deserializer.h"

namespace nemesis
{
    struct hkMeshBoneIndexMapping : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0x00,
                                                "hkMeshBoneIndexMapping",
                                                nullptr,
                                                8,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkMeshBoneIndexMapping);

        nemesis::hkArray<short> m_mapping;

    public:
        hkMeshBoneIndexMapping() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkMeshBoneIndexMapping::Class;
}
