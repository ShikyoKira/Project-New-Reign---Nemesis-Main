#include "Havok/hkpConstraintListener.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpConstraintListener,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpConstraintListener::Class; });

nemesis::hkpConstraintListener::hkpConstraintListener() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

nemesis::hkpConstraintListener::hkpConstraintListener(const nemesis::hkClass& cls) noexcept
    : nemesis::HavokObject(cls.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkpConstraintListener::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpConstraintListener::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteUSize(0); // offset: 0/0 size: 4/8 align: 4/8
    // class size: 4/8 align: 4/8
}

void nemesis::hkpConstraintListener::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadUSize(); // offset: 0/0 size: 4/8 align: 4/8
    // class size: 4/8 align: 4/8
}
