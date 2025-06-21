#pragma once

#include "Havok/hkClass.h"

#include "Serialize/Deserializer.h"

namespace nemesis
{
    struct hkAabbHalf : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0x1d716a17,
                                                "hkAabbHalf",
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

        static constexpr std::array<nemesis::hkClass, 2> Classes
            = {nemesis::hkClass{0x1d716a17,
                                "hkAabbHalf",
                                nullptr,
                                16,
                                nullptr,
                                0,
                                {},
                                {},
                                nullptr,
                                nullptr,
                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                0},
               nemesis::hkClass{0x1d716a17,
                                "hkAabbHalf",
                                nullptr,
                                16,
                                nullptr,
                                0,
                                {},
                                {},
                                nullptr,
                                nullptr,
                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                0}};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkAabbHalf);

        std::array<unsigned short, 6> m_data;
        std::array<unsigned short, 2> m_extras;

    public:
        hkAabbHalf() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkAabbHalf::Class;
    constexpr std::array<nemesis::hkClass, 2> nemesis::hkAabbHalf::Classes;
}
