#pragma once

#include "Havok/Base/hkbModifier.h"

#include "Havok/hkbRigidBodyRagdollControlData.h"
#include "Havok/hkbBoneIndexArray.h"

namespace nemesis
{
    struct hkbRigidBodyRagdollControlsModifier : nemesis::hkbModifier
    {
        static constexpr nemesis::hkClass Class{0xaa87d1eb,
                                                "hkbRigidBodyRagdollControlsModifier",
                                                &nemesis::hkbModifier::Class,
                                                160,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkbRigidBodyRagdollControlsModifier);

        nemesis::hkbRigidBodyRagdollControlData m_controlData;
        nemesis::hkRefPtr<nemesis::hkbBoneIndexArray> m_bones;
        float m_animationBlendFraction{};

    public:
        hkbRigidBodyRagdollControlsModifier() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbRigidBodyRagdollControlsModifier::Class;
}
