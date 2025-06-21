#pragma once

#include "Havok/hkaKeyFrameHierarchyUtilityControlData.h"

namespace nemesis
{
    struct hkbRigidBodyRagdollControlData : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0x1e0bc068,
                                                "hkbRigidBodyRagdollControlData",
                                                nullptr,
                                                64,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkbRigidBodyRagdollControlData);

        nemesis::hkaKeyFrameHierarchyUtilityControlData m_keyFrameHierarchyControlData; // align 16
        float m_durationToBlend;

    public:
        hkbRigidBodyRagdollControlData() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbRigidBodyRagdollControlData::Class;
}
