#pragma once

#include "Havok/Base/hkpConstraintAtom.h"
#include "Havok/Base/hkpConstraintMotor.h"

namespace nemesis
{
    struct hkpRagdollMotorConstraintAtom : nemesis::hkpConstraintAtom
    {
        static constexpr nemesis::hkClass Class{0x71013826,
                                                "hkpRagdollMotorConstraintAtom",
                                                &nemesis::hkpConstraintAtom::Class,
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
        REGISTER_HAVOK_POINTER_HEADER(hkpRagdollMotorConstraintAtom);

        bool m_isEnabled{};
        short m_initializedOffset{};
        short m_previousTargetAnglesOffset{};
        nemesis::hkMatrix3 m_target_bRca;
        std::array<nemesis::hkRefPtr<nemesis::hkpConstraintMotor>, 3> m_motors;

    public:
        hkpRagdollMotorConstraintAtom() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpRagdollMotorConstraintAtom::Class;
}
