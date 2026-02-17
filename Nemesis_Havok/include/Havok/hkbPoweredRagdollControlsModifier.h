#pragma once

#include "Havok/Base/hkbModifier.h"

#include "Havok/hkbPoweredRagdollControlData.h"
#include "Havok/hkbBoneIndexArray.h"
#include "Havok/hkbWorldFromModelModeData.h"
#include "Havok/hkbBoneWeightArray.h"

namespace nemesis
{
    struct hkbPoweredRagdollControlsModifier : nemesis::hkbModifier
    {
        static constexpr nemesis::hkClass Class{0x7cb54065,
                                                "hkbPoweredRagdollControlsModifier",
                                                &nemesis::hkbModifier::Class,
                                                144,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkbPoweredRagdollControlsModifier);

        nemesis::hkbPoweredRagdollControlData m_controlData;
        nemesis::hkRefPtr<nemesis::hkbBoneIndexArray> m_bones;
        nemesis::hkbWorldFromModelModeData m_worldFromModelModeData;
        nemesis::hkRefPtr<nemesis::hkbBoneWeightArray> m_boneWeights;
        float m_animationBlendFraction{};

    public:
        hkbPoweredRagdollControlsModifier() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbPoweredRagdollControlsModifier::Class;
}
