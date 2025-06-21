#pragma once

#include "Havok/hkClass.h"

#include "Serialize/Deserializer.h"

namespace nemesis
{
    struct hkBitField;

    struct hkBitFieldStorage : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0x00,
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
        nemesis::hkArray<unsigned int>& m_words;
        unsigned int& m_numBits;

    public:
        hkBitFieldStorage(nemesis::hkBitField& parent) noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };


    constexpr nemesis::hkClass nemesis::hkBitFieldStorage::Class;
}
