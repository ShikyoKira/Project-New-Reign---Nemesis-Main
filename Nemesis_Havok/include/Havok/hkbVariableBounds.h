#pragma once

#include "Havok/hkbVariableValue.h"

namespace nemesis
{
    struct hkbVariableBounds : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0x0b99bd6a,
                                                "hkbVariableBounds",
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
        REGISTER_HAVOK_POINTER_HEADER(hkbVariableBounds);

        nemesis::hkbVariableValue m_min;
        nemesis::hkbVariableValue m_max;

    public:
        hkbVariableBounds() noexcept;
                
        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbVariableBounds::Class;
}
