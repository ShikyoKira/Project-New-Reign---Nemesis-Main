#include "Havok/hkbCharacterControllerSetup.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbCharacterControllerSetup,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbCharacterControllerSetup::Class; });

nemesis::hkbCharacterControllerSetup::hkbCharacterControllerSetup() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkbCharacterControllerSetup::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbCharacterControllerSetup::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteObject("rigidBodySetup", m_rigidBodySetup);   // offset: 0/0 size: 24/32 align: 4/8
    serializer.WriteObject("controllerCinfo", m_controllerCinfo); // offset: 24/32 size: 4/8 align: 4/8
    // class size: 28/40 align: 4/8
}

void nemesis::hkbCharacterControllerSetup::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadObject("rigidBodySetup", m_rigidBodySetup);   // offset: 0/0 size: 24/32 align: 4/8
    deserializer.ReadObject("controllerCinfo", m_controllerCinfo); // offset: 24/32 size: 4/8 align: 4/8
    // class size: 28/40 align: 4/8
}
