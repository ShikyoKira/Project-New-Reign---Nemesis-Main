#include "Havok/BSIStateManagerModifierBSiStateData.h"

REGISTER_HAVOK_POINTER_SOURCE(BSIStateManagerModifierBSiStateData,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::BSIStateManagerModifierBSiStateData::Class; });

nemesis::BSIStateManagerModifierBSiStateData::BSIStateManagerModifierBSiStateData() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::BSIStateManagerModifierBSiStateData::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::BSIStateManagerModifierBSiStateData::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteObject("pStateMachine", m_pStateMachine); // offset: 0/0 size: 4/8 align: 4/8
    serializer.WriteValue("StateID", m_StateID);              // offset: 4/8 size: 4/4 align: 4/4
    serializer.WriteValue("iStateToSetAs", m_iStateToSetAs);  // offset: 8/12 size: 4/4 align: 4/4
    // class size: 12/16 align: 4/8
}

void nemesis::BSIStateManagerModifierBSiStateData::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadObject("pStateMachine", m_pStateMachine); // offset: 0/0 size: 4/8 align: 4/8
    deserializer.ReadValue("StateID", m_StateID);              // offset: 4/8 size: 4/4 align: 4/4
    deserializer.ReadValue("iStateToSetAs", m_iStateToSetAs);  // offset: 8/12 size: 4/4 align: 4/4
    // class size: 12/16 align: 4/8
}
