#pragma once

#include "Havok/hkClass.h"

#include "Serialize/Deserializer.h"

namespace nemesis
{
    struct hkColor : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0x106b96ce,
                                                "hkColor",
                                                nullptr,
                                                1,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkColor);

        // Must be static
        // A simple color table - arrange in batches of 8 colors light to dark
        static std::array<unsigned char, 32> s_colorTable;

        unsigned char unk0;

    public:
        hkColor() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkColor::Class;
}
