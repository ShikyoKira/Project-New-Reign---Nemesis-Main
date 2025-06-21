#pragma once

#include "Havok/hkClass.h"

#include "Serialize/Deserializer.h"

namespace nemesis
{
    struct hkpStiffSpringChainDataConstraintInfo : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0xc624a180,
                                                "hkpStiffSpringChainDataConstraintInfo",
                                                nullptr,
                                                48,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpStiffSpringChainDataConstraintInfo);

        nemesis::hkVector4 m_pivotInA;
        nemesis::hkVector4 m_pivotInB;
        float m_springLength;

    public:
        hkpStiffSpringChainDataConstraintInfo() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpStiffSpringChainDataConstraintInfo::Class;
}
