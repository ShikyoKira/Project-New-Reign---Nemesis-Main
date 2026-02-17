#pragma once

#include "Havok/hkpCharacterControllerCinfo.h"

namespace nemesis
{
    struct hkbCharacterDataCharacterControllerInfo : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0xa0f415bf,
                                                "hkbCharacterDataCharacterControllerInfo",
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
        REGISTER_HAVOK_POINTER_HEADER(hkbCharacterDataCharacterControllerInfo);

        float m_capsuleHeight{};
        float m_capsuleRadius{};
        unsigned int m_collisionFilterInfo{};
        nemesis::hkRefPtr<nemesis::hkpCharacterControllerCinfo> m_characterControllerCinfo;

    public:
        hkbCharacterDataCharacterControllerInfo() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbCharacterDataCharacterControllerInfo::Class;
}
