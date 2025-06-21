#pragma once

#include "Havok/Base/hkpWrappedConstraintData.h"

#include "Havok/hkpBridgeAtoms.h"

namespace nemesis
{
    struct hkpBreakableConstraintData : nemesis::hkpWrappedConstraintData
    {
        static constexpr nemesis::hkClass Class{0x7d6310c8,
                                                "hkpBreakableConstraintData",
                                                &nemesis::hkpWrappedConstraintData::Class,
                                                72,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpBreakableConstraintData);

        nemesis::hkpBridgeAtoms m_atoms;
        nemesis::hkRefPtr<nemesis::hkpConstraintData> m_constraintData;
        unsigned short m_childRuntimeSize;
        unsigned short m_childNumSolverResults;
        float m_solverResultLimit;
        bool m_removeWhenBroken;
        bool m_revertBackVelocityOnBreak;

    public:
        hkpBreakableConstraintData() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpBreakableConstraintData::Class;
}
