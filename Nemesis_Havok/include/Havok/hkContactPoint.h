#pragma once

#include "Havok/hkClass.h"

#include "Serialize/Deserializer.h"

namespace nemesis
{
    struct hkContactPoint : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0x91d7dd8e,
                                                "hkContactPoint",
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
        REGISTER_HAVOK_POINTER_HEADER(hkContactPoint);

        nemesis::hkVector4 m_position;
        nemesis::hkVector4 m_separatingNormal;

    public:
        hkContactPoint() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkContactPoint::Class;
}
