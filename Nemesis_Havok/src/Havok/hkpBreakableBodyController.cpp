#include "Havok/hkpBreakableBodyController.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpBreakableBodyController,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpBreakableBodyController::Class; });

nemesis::hkpBreakableBodyController::hkpBreakableBodyController() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkpBreakableBodyController::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpBreakableBodyController::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkReferencedObject::SerializeTo(serializer);        // offset: 0/0 size: 8/16 align: 4/8
    serializer.WriteValue("breakingImpulse", m_breakingImpulse); // offset: 8/16 size: 4/4 align: 4/4
    serializer.Pad(serializer.GetPointerSize());                 // offset: 12/20 size: 0/4
    // class size: 12/24 align: 4/8
}

void nemesis::hkpBreakableBodyController::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkReferencedObject::DeserializeFrom(deserializer);   // offset: 0/0 size: 8/16 align: 4/8
    deserializer.ReadValue("breakingImpulse", m_breakingImpulse); // offset: 8/16 size: 4/4 align: 4/4
    deserializer.Pad(deserializer.GetPointerSize());              // offset: 12/20 size: 0/4
    // class size: 12/24 align: 4/8
}
