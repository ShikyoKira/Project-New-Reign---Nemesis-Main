#include "Havok/hkpShapeCollection.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpShapeCollection,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpShapeCollection::Class; });

nemesis::hkpShapeCollection::hkpShapeCollection() noexcept
    : nemesis::hkpShapeCollection(Class)
{
}

nemesis::hkpShapeCollection::hkpShapeCollection(const nemesis::hkClass& cls) noexcept
    : nemesis::HavokObject(cls.GetSignature())
    , nemesis::hkpShape(cls)
    , nemesis::hkpShapeContainer(cls)
{
}

const nemesis::hkClass* nemesis::hkpShapeCollection::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpShapeCollection::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpShape::SerializeTo(serializer);                // offset: 0/0 size: 16/32 align: 4/8
    nemesis::hkpShapeContainer::SerializeTo(serializer);       // offset: 16/32 size: 4/8 align: 4/8
    serializer.WriteValue("disableWelding", m_disableWelding); // offset: 20/40 size: 1/1 align: 1/1
    serializer.WriteValue("collectionType", m_collectionType); // offset: 21/41 size: 1/1 align: 1/1
    serializer.Pad(serializer.GetPointerSize());               // offset: 22/42 size: 2/6
    // class size: 24/48 align: 4/8
}

void nemesis::hkpShapeCollection::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpShape::DeserializeFrom(deserializer);           // offset: 0/0 size: 16/32 align: 4/8
    nemesis::hkpShapeContainer::DeserializeFrom(deserializer);  // offset: 16/32 size: 4/8 align: 4/8
    deserializer.ReadValue("disableWelding", m_disableWelding); // offset: 20/40 size: 1/1 align: 1/1
    deserializer.ReadValue("collectionType", m_collectionType); // offset: 21/41 size: 1/1 align: 1/1
    deserializer.Pad(deserializer.GetPointerSize());            // offset: 22/42 size: 2/6
    // class size: 24/48 align: 4/8
}
