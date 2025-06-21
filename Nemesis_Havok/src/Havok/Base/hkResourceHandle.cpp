#include "Havok/Base/hkResourceHandle.h"

nemesis::hkResourceHandle::hkResourceHandle(const nemesis::hkClass& cls) noexcept
    : nemesis::HavokObject(cls.GetSignature())
    , nemesis::hkResourceBase(cls)
{
}

void nemesis::hkResourceHandle::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkResourceBase::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8
    // class size: 8/16 align: 4/8
}

void nemesis::hkResourceHandle::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkResourceBase::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    // class size: 8/16 align: 4/8
}
