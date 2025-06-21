#pragma once

#include "Havok/hkClass.h"

#include "Serialize/Deserializer.h"

namespace nemesis
{
    struct hkAabbUint32 : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0x11e7c11,
                                                "hkAabbUint32",
                                                nullptr,
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
        REGISTER_HAVOK_POINTER_HEADER(hkAabbUint32);

        std::array<unsigned int, 3> m_min; // align 16
        std::array<unsigned char, 3> m_expansionMin;
        unsigned char m_expansionShift;
        std::array<unsigned int, 3> m_max;
        std::array<unsigned char, 3> m_expansionMax;
        unsigned char m_shapeKeyByte;

    public:
        hkAabbUint32() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkAabbUint32::Class;
}
