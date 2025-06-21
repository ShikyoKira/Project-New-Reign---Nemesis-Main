#include "Havok/hkbExpressionDataArray.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbExpressionDataArray,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbExpressionDataArray::Class; });

nemesis::hkbExpressionDataArray::hkbExpressionDataArray() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkbExpressionDataArray::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbExpressionDataArray::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkReferencedObject::SerializeTo(serializer);         // offset: 0/0 size: 8/16 align: 4/8
    serializer.WriteObject("expressionsData", m_expressionsData); // offset: 8/16 size: 12/16 align: 4/8
    // class size: 20/32 align: 4/8
}

void nemesis::hkbExpressionDataArray::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkReferencedObject::DeserializeFrom(deserializer);    // offset: 0/0 size: 8/16 align: 4/8
    deserializer.ReadObject("expressionsData", m_expressionsData); // offset: 8/16 size: 12/16 align: 4/8
    // class size: 20/32 align: 4/8
}
