#pragma once

#include "Havok/Base/hkbModifier.h"

namespace nemesis
{
    struct hkbExtractRagdollPoseModifier : nemesis::hkbModifier
    {
        static constexpr nemesis::hkClass Class{0x804dcbab,
                                                "hkbExtractRagdollPoseModifier",
                                                &nemesis::hkbModifier::Class,
                                                88,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkbExtractRagdollPoseModifier);

        short m_poseMatchingBone0;
        short m_poseMatchingBone1;
        short m_poseMatchingBone2;
        bool m_enableComputeWorldFromModel;

    public:
        hkbExtractRagdollPoseModifier() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbExtractRagdollPoseModifier::Class;
}
