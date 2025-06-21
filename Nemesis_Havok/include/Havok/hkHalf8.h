#pragma once

#include "Havok/hkClass.h"

#include "Havok/Types/hkHalf.h"

#include "Serialize/Deserializer.h"

namespace nemesis
{
    struct hkHalf8 : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0x7684dc80,
                                                "hkHalf8",
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
        REGISTER_HAVOK_POINTER_HEADER(hkHalf8);

        std::array<nemesis::hkHalf, 8> m_quad; // align 16
        std::array<short, 8> m_halfs;          // align 16

    public:
        hkHalf8() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkHalf8::Class;
}
