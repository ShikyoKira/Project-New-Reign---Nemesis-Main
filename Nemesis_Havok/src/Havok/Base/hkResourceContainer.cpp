#include "Havok/Base/hkResourceContainer.h"

nemesis::hkResourceContainer::hkResourceContainer(const nemesis::hkClass& cls) noexcept
    : nemesis::HavokObject(cls.GetSignature())
    , nemesis::hkResourceBase(cls)
{
}

void nemesis::hkResourceContainer::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkResourceBase::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8
    // class size: 8/16 align: 4/8
}

void nemesis::hkResourceContainer::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkResourceBase::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    // class size: 8/16 align: 4/8
}
