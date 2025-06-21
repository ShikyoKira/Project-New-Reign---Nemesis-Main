#pragma once

#include "Havok/hkbFootIkGains.h"

namespace nemesis
{
    struct hkbFootIkControlData : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0xa111b704,
                                                "hkbFootIkControlData",
                                                nullptr,
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
        REGISTER_HAVOK_POINTER_HEADER(hkbFootIkControlData);

        nemesis::hkbFootIkGains m_gains; // align 16
        std::array<nemesis::hkVector8, 2> m_enabled;

    public:
        hkbFootIkControlData() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbFootIkControlData::Class;
}
