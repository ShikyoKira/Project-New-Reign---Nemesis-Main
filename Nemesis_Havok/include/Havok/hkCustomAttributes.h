#pragma once

#include "Havok/hkClass.h"

#include "Serialize/Deserializer.h"

namespace nemesis
{
    struct hkCustomAttributes : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0xbff19005,
                                                "hkCustomAttributes",
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
        REGISTER_HAVOK_POINTER_HEADER(hkCustomAttributes);

        nemesis::hkRefPtr<nemesis::HavokObject> m_attributes;

    public:
        hkCustomAttributes() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkCustomAttributes::Class;
}
