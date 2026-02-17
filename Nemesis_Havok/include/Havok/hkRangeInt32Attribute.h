#pragma once

#include "Havok/hkClass.h"

#include "Serialize/Deserializer.h"

namespace nemesis
{
    struct hkRangeInt32Attribute : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0x4846be29,
                                                "hkRangeInt32Attribute",
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
        REGISTER_HAVOK_POINTER_HEADER(hkRangeInt32Attribute);

        int m_absmin{};
        int m_absmax{};
        int m_softmin{};
        int m_softmax{};

    public:
        hkRangeInt32Attribute() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkRangeInt32Attribute::Class;
}
