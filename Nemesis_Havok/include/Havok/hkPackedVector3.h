#pragma once

#include "Havok/hkClass.h"

#include "Serialize/Deserializer.h"

namespace nemesis
{
    struct hkPackedVector3 : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0x9c16df5b,
                                                "hkPackedVector3",
                                                nullptr,
                                                8,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkPackedVector3);

        std::array<short, 4> m_values{};

    public:
        hkPackedVector3() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkPackedVector3::Class;
}
