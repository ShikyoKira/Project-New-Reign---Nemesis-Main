#pragma once

#include "Havok/Base/hkpConstraintMotor.h"

namespace nemesis
{
    struct hkpPoweredChainDataConstraintInfo : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0xf88aee25,
                                                "hkpPoweredChainDataConstraintInfo",
                                                nullptr,
                                                96,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpPoweredChainDataConstraintInfo);

        nemesis::hkVector4 m_pivotInA;
        nemesis::hkVector4 m_pivotInB;
        nemesis::hkQuaternion m_aTc;
        nemesis::hkQuaternion m_bTc;
        std::array<nemesis::hkRefPtr<nemesis::hkpConstraintMotor>, 3> m_motors;
        bool m_switchBodies;

    public:
        hkpPoweredChainDataConstraintInfo() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpPoweredChainDataConstraintInfo::Class;
}
