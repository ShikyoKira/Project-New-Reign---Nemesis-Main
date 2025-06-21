#include "Havok/hkbSequencedData.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbSequencedData,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbSequencedData::Class; });

nemesis::hkbSequencedData::hkbSequencedData() noexcept
    : nemesis::hkbSequencedData(Class)
{
}

nemesis::hkbSequencedData::hkbSequencedData(const nemesis::hkClass& cls) noexcept
    : nemesis::HavokObject(cls.GetSignature())
    , nemesis::hkReferencedObject(cls)
{
}

const nemesis::hkClass* nemesis::hkbSequencedData::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbSequencedData::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkReferencedObject::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8
    // class size: 8/16 align: 4/8
}

void nemesis::hkbSequencedData::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    // class size: 8/16 align: 4/8
}
