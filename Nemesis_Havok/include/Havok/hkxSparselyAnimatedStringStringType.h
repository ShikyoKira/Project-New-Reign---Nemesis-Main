#pragma once

#include "Havok/hkClass.h"

#include "Serialize/Deserializer.h"

namespace nemesis
{
    struct hkxSparselyAnimatedString;

    struct hkxSparselyAnimatedStringStringType : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0x00,
                                                "hkxSparselyAnimatedString",
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
        REGISTER_HAVOK_POINTER_HEADER(hkxSparselyAnimatedStringStringType);

        nemesis::hkStringPtr m_string;

    public:
        hkxSparselyAnimatedStringStringType() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;

        friend nemesis::hkxSparselyAnimatedString;
    };

    constexpr nemesis::hkClass nemesis::hkxSparselyAnimatedStringStringType::Class;
}
