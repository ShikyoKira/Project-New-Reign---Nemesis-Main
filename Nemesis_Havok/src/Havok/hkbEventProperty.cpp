#include "Havok/hkbEventProperty.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbEventProperty,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbEventProperty::Class; });

nemesis::hkbEventProperty::hkbEventProperty() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkbEventBase(Class)
{
}

const nemesis::hkClass* nemesis::hkbEventProperty::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbEventProperty::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkbEventBase::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8
    // class size: 8/16 align: 4/8
}

void nemesis::hkbEventProperty::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkbEventBase::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    // class size: 8/16 align: 4/8
}
