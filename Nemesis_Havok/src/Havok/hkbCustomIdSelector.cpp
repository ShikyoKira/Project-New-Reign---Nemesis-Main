#include "Havok/hkbCustomIdSelector.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbCustomIdSelector,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbCustomIdSelector::Class; });

nemesis::hkbCustomIdSelector::hkbCustomIdSelector() noexcept
    : nemesis::hkbCustomIdSelector(Class)
{
}

nemesis::hkbCustomIdSelector::hkbCustomIdSelector(const nemesis::hkClass& cls) noexcept
    : nemesis::HavokObject(cls.GetSignature())
    , nemesis::hkReferencedObject(cls)
{
}

const nemesis::hkClass* nemesis::hkbCustomIdSelector::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbCustomIdSelector::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkReferencedObject::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8
    // class size: 8/16 align: 4/8
}

void nemesis::hkbCustomIdSelector::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    // class size: 8/16 align: 4/8
}
