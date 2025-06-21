#include "Havok/hkpRemoveTerminalsMoppModifier.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpRemoveTerminalsMoppModifier,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpRemoveTerminalsMoppModifier::Class; });

nemesis::hkpRemoveTerminalsMoppModifier::hkpRemoveTerminalsMoppModifier() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
    , nemesis::hkpMoppModifier(Class)
{
}

const nemesis::hkClass* nemesis::hkpRemoveTerminalsMoppModifier::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpRemoveTerminalsMoppModifier::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkReferencedObject::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8
    nemesis::hkpMoppModifier::SerializeTo(serializer);    // offset: 8/16 size: 4/8 align: 4/8
    serializer.WriteObject("removeInfo", m_removeInfo);   // offset: 12/24 size: 12/16 align: 4/8
    serializer.WriteSerializeIgnoredObject("tempShapesToRemove",
                                           m_tempShapesToRemove); // offset: 24/40 size: 4/8 align: 4/8
    // class size: 28/48 align: 4/8
}

void nemesis::hkpRemoveTerminalsMoppModifier::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    nemesis::hkpMoppModifier::DeserializeFrom(deserializer);    // offset: 8/16 size: 4/8 align: 4/8
    deserializer.ReadObject("removeInfo", m_removeInfo);        // offset: 12/24 size: 12/16 align: 4/8
    deserializer.ReadObject("tempShapesToRemove", m_tempShapesToRemove); // offset: 24/40 size: 4/8 align: 4/8
    // class size: 28/48 align: 4/8
}
