#pragma once

#include "Havok/Base/hkReferencedObject.h"

namespace nemesis
{
    struct hkbCharacterInfo : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0xd9709ff2,
                                                "hkbCharacterInfo",
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
        REGISTER_HAVOK_POINTER_HEADER(hkbCharacterInfo);

        using Event = nemesis::hkInternalClassEnumItem<unsigned char>;

        uint64_t m_characterId;
        nemesis::hkStringPtr m_instanceName;
        nemesis::hkStringPtr m_templateName;
        nemesis::hkStringPtr m_fullPathToProject;
        nemesis::hkEnum<Event, unsigned char> m_event = {
            {"REMOVED_FROM_WORLD", 0},
            {"SHOWN", 1},
            {"HIDDEN", 2},
            {"ACTIVATED", 3},
            {"DEACTIVATED", 4},
        };
        int m_padding;

    public:
        hkbCharacterInfo() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbCharacterInfo::Class;
}
