#pragma once

#include "Havok/hkbAttachmentSetup.h"
#include "Havok/hkbProjectStringData.h"

namespace nemesis
{
    struct hkbProjectData : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0x13a39ba7,
                                                "hkbProjectData",
                                                &nemesis::hkReferencedObject::Class,
                                                48,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkbProjectData);

        using EventMode = nemesis::hkInternalClassEnumItem<char>;

        nemesis::hkArray<nemesis::hkbAttachmentSetup> m_attachmentSetups;
        nemesis::hkVector4 m_worldUpWS;
        nemesis::hkRefPtr<nemesis::hkbProjectStringData> m_stringData;
        nemesis::hkEnum<EventMode, char> m_defaultEventMode = {
            {"EVENT_MODE_DEFAULT", 0},
            {"EVENT_MODE_PROCESS_ALL", 1},
            {"EVENT_MODE_IGNORE_FROM_GENERATOR", 2},
            {"EVENT_MODE_IGNORE_TO_GENERATOR", 3},
        };

    public:
        hkbProjectData() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbProjectData::Class;
}
