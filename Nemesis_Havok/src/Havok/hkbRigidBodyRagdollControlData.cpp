#include "Havok/hkbRigidBodyRagdollControlData.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbRigidBodyRagdollControlData,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbRigidBodyRagdollControlData::Class; });

nemesis::hkbRigidBodyRagdollControlData::hkbRigidBodyRagdollControlData() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkbRigidBodyRagdollControlData::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbRigidBodyRagdollControlData::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteObject(serializer.GetContentsVersion() > HavokVersion::HK_2012_2_0
                               ? "keyFrameControlData"
                               : "keyFrameHierarchyControlData",
                           m_keyFrameHierarchyControlData);      // offset: 0/0 size: 48/48 align: 16/16
    serializer.WriteValue("durationToBlend", m_durationToBlend); // offset: 48/48 size: 4/4 align: 4/4
    serializer.Skip(12);                                         // offset: 52/52 size: 12/12
    // class size: 64/64 align: 16/16
}

void nemesis::hkbRigidBodyRagdollControlData::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadObject(deserializer.GetContentsVersion() > HavokVersion::HK_2012_2_0
                                ? "keyFrameControlData"
                                : "keyFrameHierarchyControlData",
                            m_keyFrameHierarchyControlData);      // offset: 0/0 size: 48/48 align: 16/16
    deserializer.ReadValue("durationToBlend", m_durationToBlend); // offset: 48/48 size: 4/4 align: 4/4
    deserializer.Skip(12);                                        // offset: 52/52 size: 12/12
    // class size: 64/64 align: 16/16
}
