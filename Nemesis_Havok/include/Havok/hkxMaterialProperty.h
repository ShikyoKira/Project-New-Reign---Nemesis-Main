#pragma once

#include "Havok/hkClass.h"

#include "Serialize/Deserializer.h"

namespace nemesis
{
    struct hkxMaterialProperty : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0xd295234d,
                                                "hkxMaterialProperty",
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
        REGISTER_HAVOK_POINTER_HEADER(hkxMaterialProperty);

        unsigned int m_key{};
        unsigned int m_value{};

    public:
        hkxMaterialProperty() noexcept;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkxMaterialProperty::Class;
}
