#include "Havok/hkpSingleShapeContainer.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpSingleShapeContainer,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpSingleShapeContainer::Class; });

nemesis::hkpSingleShapeContainer::hkpSingleShapeContainer() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkpShapeContainer(Class)
{
}

const nemesis::hkClass* nemesis::hkpSingleShapeContainer::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpSingleShapeContainer::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpShapeContainer::SerializeTo(serializer); // offset: 0/0 size: 8/8 align: 1/1
    serializer.WriteObject("childShape", m_childShape);  // offset: 8/8 size: 4/8 align: 4/8
    // class size: 12/16 align: 4/8
}

void nemesis::hkpSingleShapeContainer::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpShapeContainer::DeserializeFrom(deserializer); // offset: 0/0 size: 8/8 align: 1/1
    deserializer.ReadObject("childShape", m_childShape);       // offset: 8/8 size: 4/8 align: 4/8
    // class size: 12/16 align: 4/8
}
