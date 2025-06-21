#include "Havok/hkbCompiledExpressionSet.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbCompiledExpressionSet,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbCompiledExpressionSet::Class; });

nemesis::hkbCompiledExpressionSet::hkbCompiledExpressionSet() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkbCompiledExpressionSet::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbCompiledExpressionSet::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkReferencedObject::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8
    serializer.WriteObject("rpn", m_rpn);                 // offset: 8/16 size: 12/16 align: 4/8
    serializer.WriteObject("expressionToRpnIndex",
                           m_expressionToRpnIndex);            // offset: 20/32 size: 12/16 align: 4/8
    serializer.WriteValue("numExpressions", m_numExpressions); // offset: 32/48 size: 1/1 align: 1/1
    serializer.Pad(serializer.GetPointerSize());               // offset: 33/49 size: 3/7
    // class size: 36/56 align: 4/8
}

void nemesis::hkbCompiledExpressionSet::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    deserializer.ReadObject("rpn", m_rpn);                      // offset: 8/16 size: 12/16 align: 4/8
    deserializer.ReadObject("expressionToRpnIndex",
                            m_expressionToRpnIndex);            // offset: 20/32 size: 12/16 align: 4/8
    deserializer.ReadValue("numExpressions", m_numExpressions); // offset: 32/48 size: 1/1 align: 1/1
    deserializer.Pad(deserializer.GetPointerSize());            // offset: 33/49 size: 3/7
    // class size: 36/56 align: 4/8
}
