#pragma once

#include "Havok/hkClass.h"

#include "Serialize/Deserializer.h"

namespace nemesis
{
    struct hkCustomAttributesAttribute : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0x1388d601,
                                                "hkCustomAttributesAttribute",
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
        REGISTER_HAVOK_POINTER_HEADER(hkCustomAttributesAttribute);

        nemesis::hkCString m_name;
        nemesis::hkRefPtr<nemesis::HavokObject> m_value;

    public:
        hkCustomAttributesAttribute() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkCustomAttributesAttribute::Class;
}
