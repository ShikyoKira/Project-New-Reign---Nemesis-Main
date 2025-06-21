#include "Havok/hkaKeyFrameHierarchyUtilityControlData.h"

REGISTER_HAVOK_POINTER_SOURCE(hkaKeyFrameHierarchyUtilityControlData,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkaKeyFrameHierarchyUtilityControlData::Class; });

nemesis::hkaKeyFrameHierarchyUtilityControlData::hkaKeyFrameHierarchyUtilityControlData() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkaKeyFrameHierarchyUtilityControlData::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkaKeyFrameHierarchyUtilityControlData::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("hierarchyGain", m_hierarchyGain);       // offset: 0/0 size: 4/4 align: 4/4
    serializer.WriteValue("velocityDamping", m_velocityDamping);   // offset: 4/4 size: 4/4 align: 4/4
    serializer.WriteValue("accelerationGain", m_accelerationGain); // offset: 8/8 size: 4/4 align: 4/4
    serializer.WriteValue("velocityGain", m_velocityGain);         // offset: 12/12 size: 4/4 align: 4/4
    serializer.WriteValue("positionGain", m_positionGain);         // offset: 16/16 size: 4/4 align: 4/4
    serializer.WriteValue("positionMaxLinearVelocity",
                          m_positionMaxLinearVelocity); // offset: 20/20 size: 4/4 align: 4/4
    serializer.WriteValue("positionMaxAngularVelocity",
                          m_positionMaxAngularVelocity); // offset: 24/24 size: 4/4 align: 4/4
    serializer.WriteValue("snapGain", m_snapGain);       // offset: 28/28 size: 4/4 align: 4/4
    serializer.WriteValue("snapMaxLinearVelocity",
                          m_snapMaxLinearVelocity); // offset: 32/32 size: 4/4 align: 4/4
    serializer.WriteValue("snapMaxAngularVelocity",
                          m_snapMaxAngularVelocity); // offset: 36/36 size: 4/4 align: 4/4
    serializer.WriteValue("snapMaxLinearDistance",
                          m_snapMaxLinearDistance); // offset: 40/40 size: 4/4 align: 4/4
    serializer.WriteValue("snapMaxAngularDistance",
                          m_snapMaxAngularDistance); // offset: 44/44 size: 4/4 align: 4/4
    // class size: 48/48 align: 4/4
}

void nemesis::hkaKeyFrameHierarchyUtilityControlData::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("hierarchyGain", m_hierarchyGain);       // offset: 0/0 size: 4/4 align: 4/4
    deserializer.ReadValue("velocityDamping", m_velocityDamping);   // offset: 4/4 size: 4/4 align: 4/4
    deserializer.ReadValue("accelerationGain", m_accelerationGain); // offset: 8/8 size: 4/4 align: 4/4
    deserializer.ReadValue("velocityGain", m_velocityGain);         // offset: 12/12 size: 4/4 align: 4/4
    deserializer.ReadValue("positionGain", m_positionGain);         // offset: 16/16 size: 4/4 align: 4/4
    deserializer.ReadValue("positionMaxLinearVelocity",
                           m_positionMaxLinearVelocity); // offset: 20/20 size: 4/4 align: 4/4
    deserializer.ReadValue("positionMaxAngularVelocity",
                           m_positionMaxAngularVelocity); // offset: 24/24 size: 4/4 align: 4/4
    deserializer.ReadValue("snapGain", m_snapGain);       // offset: 28/28 size: 4/4 align: 4/4
    deserializer.ReadValue("snapMaxLinearVelocity",
                           m_snapMaxLinearVelocity); // offset: 32/32 size: 4/4 align: 4/4
    deserializer.ReadValue("snapMaxAngularVelocity",
                           m_snapMaxAngularVelocity); // offset: 36/36 size: 4/4 align: 4/4
    deserializer.ReadValue("snapMaxLinearDistance",
                           m_snapMaxLinearDistance); // offset: 40/40 size: 4/4 align: 4/4
    deserializer.ReadValue("snapMaxAngularDistance",
                           m_snapMaxAngularDistance); // offset: 44/44 size: 4/4 align: 4/4
    // class size: 48/48 align: 4/4
}
