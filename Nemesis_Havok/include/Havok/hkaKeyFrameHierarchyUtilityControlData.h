#pragma once

#include "Havok/hkClass.h"

#include "Serialize/Deserializer.h"

namespace nemesis
{
    struct hkaKeyFrameHierarchyUtilityControlData : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0xa3d0ac71,
                                                "hkaKeyFrameHierarchyUtilityControlData",
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
        REGISTER_HAVOK_POINTER_HEADER(hkaKeyFrameHierarchyUtilityControlData);

        float m_hierarchyGain;
        float m_velocityDamping;
        float m_accelerationGain;
        float m_velocityGain;
        float m_positionGain;
        float m_positionMaxLinearVelocity;
        float m_positionMaxAngularVelocity;
        float m_snapGain;
        float m_snapMaxLinearVelocity;
        float m_snapMaxAngularVelocity;
        float m_snapMaxLinearDistance;
        float m_snapMaxAngularDistance;

    public:
        hkaKeyFrameHierarchyUtilityControlData() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkaKeyFrameHierarchyUtilityControlData::Class;
}
