#include "Havok/BSIStateManagerModifierBSIStateManagerStateListener.h"

nemesis::BSIStateManagerModifierBSIStateManagerStateListener::
    BSIStateManagerModifierBSIStateManagerStateListener() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkbStateListener(Class)
{
}

const nemesis::hkClass* nemesis::BSIStateManagerModifierBSIStateManagerStateListener::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::BSIStateManagerModifierBSIStateManagerStateListener::SerializeTo(
    nemesis::Serializer& serializer) const
{
    nemesis::hkbStateListener::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8
    serializer.WriteSerializeIgnoredObject("pStateManager",
                                           m_pStateManager); // offset: 8/16 size: 4/8 align: 4/8
    // class size: 12/24 align: 4/8
}

void nemesis::BSIStateManagerModifierBSIStateManagerStateListener::DeserializeFrom(
    nemesis::Deserializer& deserializer)
{
    nemesis::hkbStateListener::DeserializeFrom(deserializer);  // offset: 0/0 size: 8/16 align: 4/8
    deserializer.ReadObject("pStateManager", m_pStateManager); // offset: 8/16 size: 4/8 align: 4/8
    // class size: 12/24 align: 4/8
}
