#pragma once

#include "Havok/hkClass.h"

#include "Serialize/Deserializer.h"

namespace nemesis
{
    struct hkbCharacterStringDataFileNameMeshNamePair : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0x26c3a96,
                                                "hkbCharacterStringDataFileNameMeshNamePair",
                                                nullptr,
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
        REGISTER_HAVOK_POINTER_HEADER(hkbCharacterStringDataFileNameMeshNamePair);

        nemesis::hkStringPtr m_fileName;
        nemesis::hkStringPtr m_meshName;

    public:
        hkbCharacterStringDataFileNameMeshNamePair() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbCharacterStringDataFileNameMeshNamePair::Class;
}
