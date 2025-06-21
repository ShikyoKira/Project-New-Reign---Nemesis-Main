#pragma once

#include "Havok/hkClass.h"

#include "Serialize/Deserializer.h"

namespace nemesis
{
    struct hkSemanticsAttribute : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0x837099c3,
                                                "hkSemanticsAttribute",
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
        REGISTER_HAVOK_POINTER_HEADER(hkSemanticsAttribute);

        using Semantics = nemesis::hkInternalClassEnumItem<char>;

        nemesis::hkEnum<Semantics, char> m_type = {
            {"UNKNOWN", 0},
            {"DISTANCE", 1},
            {"ANGLE", 2},
            {"NORMAL", 3},
            {"POSITION", 4},
            {"COSINE_ANGLE", 5},
        };

    public:
        hkSemanticsAttribute() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkSemanticsAttribute::Class;
}
