#include "Havok/hkbEvaluateExpressionModifierInternalState.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbEvaluateExpressionModifierInternalState,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbEvaluateExpressionModifierInternalState::Class; });

nemesis::hkbEvaluateExpressionModifierInternalState::hkbEvaluateExpressionModifierInternalState() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkbEvaluateExpressionModifierInternalState::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbEvaluateExpressionModifierInternalState::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkReferencedObject::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8
    serializer.WriteObject("internalExpressionsData",
                           m_internalExpressionsData); // offset: 8/16 size: 12/16 align: 4/8
    // class size: 20/32 align: 4/8
}

void nemesis::hkbEvaluateExpressionModifierInternalState::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    deserializer.ReadObject("internalExpressionsData",
                            m_internalExpressionsData); // offset: 8/16 size: 12/16 align: 4/8
    // class size: 20/32 align: 4/8
}
