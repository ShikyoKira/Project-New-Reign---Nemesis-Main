#include "Havok/hkbWorldFromModelModeData.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbWorldFromModelModeData,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbWorldFromModelModeData::Class; });

nemesis::hkbWorldFromModelModeData::hkbWorldFromModelModeData() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkbWorldFromModelModeData::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbWorldFromModelModeData::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("poseMatchingBone0", m_poseMatchingBone0); // offset: 0/0 size: 2/2 align: 2/2
    serializer.WriteValue("poseMatchingBone1", m_poseMatchingBone1); // offset: 2/2 size: 2/2 align: 2/2
    serializer.WriteValue("poseMatchingBone2", m_poseMatchingBone2); // offset: 4/4 size: 2/2 align: 2/2
    serializer.WriteValue("mode", m_mode);                           // offset: 6/6 size: 1/1 align: 1/1
    serializer.Skip(1);                                              // offset: 7/7 size: 1/1
    // class size: 8/8 align: 2/2
}

void nemesis::hkbWorldFromModelModeData::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("poseMatchingBone0", m_poseMatchingBone0); // offset: 0/0 size: 2/2 align: 2/2
    deserializer.ReadValue("poseMatchingBone1", m_poseMatchingBone1); // offset: 2/2 size: 2/2 align: 2/2
    deserializer.ReadValue("poseMatchingBone2", m_poseMatchingBone2); // offset: 4/4 size: 2/2 align: 2/2
    deserializer.ReadValue("mode", m_mode);                           // offset: 6/6 size: 1/1 align: 1/1
    deserializer.Skip(1);                                             // offset: 7/7 size: 1/1
    // class size: 8/8 align: 2/2
}
