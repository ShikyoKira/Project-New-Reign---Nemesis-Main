#pragma once

#include "Havok/hkClass.h"

#include "Serialize/Deserializer.h"

namespace nemesis
{
    struct hkxEnvironmentVariable : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0xa6815115,
                                                "hkxEnvironmentVariable",
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
        REGISTER_HAVOK_POINTER_HEADER(hkxEnvironmentVariable);

        nemesis::hkStringPtr m_name;
        nemesis::hkStringPtr m_value;

    public:
        hkxEnvironmentVariable() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkxEnvironmentVariable::Class;
}
