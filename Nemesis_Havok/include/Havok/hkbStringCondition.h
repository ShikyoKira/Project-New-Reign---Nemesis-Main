#pragma once

#include "Havok/Base/hkbCondition.h"

namespace nemesis
{
    struct hkbStringCondition : nemesis::hkbCondition
    {
        static constexpr nemesis::hkClass Class{0x5ab50487,
                                                "hkbStringCondition",
                                                &nemesis::hkbCondition::Class,
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
        REGISTER_HAVOK_POINTER_HEADER(hkbStringCondition);

        nemesis::hkStringPtr m_conditionString;

    public:
        hkbStringCondition() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbStringCondition::Class;
}
