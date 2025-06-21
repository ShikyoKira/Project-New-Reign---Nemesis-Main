#pragma once

#include "Havok/hkClass.h"

#include "Serialize/Deserializer.h"

namespace nemesis
{
    struct hkRangeRealAttribute : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0x949db24f,
                                                "hkRangeRealAttribute",
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
        REGISTER_HAVOK_POINTER_HEADER(hkRangeRealAttribute);

        float m_absmin;
        float m_absmax;
        float m_softmin;
        float m_softmax;

    public:
        hkRangeRealAttribute() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkRangeRealAttribute::Class;
}
