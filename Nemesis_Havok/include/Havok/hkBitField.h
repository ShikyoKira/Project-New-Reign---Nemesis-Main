#pragma once

#include "Havok/hkBitFieldStorage.h"

namespace nemesis
{
    struct hkBitField : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0xda41bd9b,
                                                "hkBitField",
                                                nullptr,
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
        REGISTER_HAVOK_POINTER_HEADER(hkBitField);

        nemesis::hkArray<unsigned int> m_words;
        unsigned int m_numBits{};
        hkBitFieldStorage m_storage;

    public:
        hkBitField() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;

        friend nemesis::hkBitFieldStorage;
    };

    constexpr nemesis::hkClass nemesis::hkBitField::Class;
}
