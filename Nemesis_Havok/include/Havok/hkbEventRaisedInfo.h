#pragma once

#include "Havok/Base/hkReferencedObject.h"

namespace nemesis
{
    struct hkbEventRaisedInfo : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0xc02da3,
                                                "hkbEventRaisedInfo",
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
        REGISTER_HAVOK_POINTER_HEADER(hkbEventRaisedInfo);

        uint64_t m_characterId;
        nemesis::hkStringPtr m_eventName;
        bool m_raisedBySdk;
        int m_senderId;
        int m_padding;

    public:
        hkbEventRaisedInfo() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbEventRaisedInfo::Class;
}
