#include "Havok/hkaKeyFrameHierarchyUtility.h"

REGISTER_HAVOK_POINTER_SOURCE(hkaKeyFrameHierarchyUtility,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkaKeyFrameHierarchyUtility::Class; });

nemesis::hkaKeyFrameHierarchyUtility::hkaKeyFrameHierarchyUtility() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkaKeyFrameHierarchyUtility::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkaKeyFrameHierarchyUtility::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("", unk0); // offset: 0/0 size: 1/1 align: 1/1
    // class size: 1/1 align: 1/1
}

void nemesis::hkaKeyFrameHierarchyUtility::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("", unk0); // offset: 0/0 size: 1/1 align: 1/1
    // class size: 1/1 align: 1/1
}
