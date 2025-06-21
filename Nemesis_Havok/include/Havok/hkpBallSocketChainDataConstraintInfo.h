#pragma once

#include "Havok/hkClass.h"

#include "Serialize/Deserializer.h"

namespace nemesis
{
    struct hkpBallSocketChainDataConstraintInfo : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0xc9cbedf2,
                                                "hkpBallSocketChainDataConstraintInfo",
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
        REGISTER_HAVOK_POINTER_HEADER(hkpBallSocketChainDataConstraintInfo);

        nemesis::hkVector4 m_pivotInA;
        nemesis::hkVector4 m_pivotInB;

    public:
        hkpBallSocketChainDataConstraintInfo() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpBallSocketChainDataConstraintInfo::Class;
}
