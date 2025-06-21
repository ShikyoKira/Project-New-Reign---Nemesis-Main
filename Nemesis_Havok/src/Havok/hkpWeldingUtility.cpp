#include "Havok/hkpWeldingUtility.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpWeldingUtility,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpWeldingUtility::Class; });

nemesis::hkpWeldingUtility::hkpWeldingUtility() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkpWeldingUtility::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpWeldingUtility::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("", unk0); // offset: 0/0 size: 1/1 align: 1/1
    // class size: 1/1 align: 1/1
}

void nemesis::hkpWeldingUtility::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("", unk0); // offset: 0/0 size: 1/1 align: 1/1
    // class size: 1/1 align: 1/1
}
