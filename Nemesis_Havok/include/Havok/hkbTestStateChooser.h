#pragma once

#include "Havok/hkbStateChooser.h"

namespace nemesis
{
    struct hkbTestStateChooser : nemesis::hkbStateChooser
    {
        static constexpr nemesis::hkClass Class{0xc0fcc436,
                                                "hkbTestStateChooser",
                                                &nemesis::hkbStateChooser::Class,
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
        REGISTER_HAVOK_POINTER_HEADER(hkbTestStateChooser);

        int m_int;
        float m_real;
        nemesis::hkStringPtr m_string;

    public:
        hkbTestStateChooser() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbTestStateChooser::Class;
}
