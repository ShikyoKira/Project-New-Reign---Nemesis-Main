#include "Havok/Base/hkpAction.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpAction,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpAction::Class; });

nemesis::hkpAction::hkpAction() noexcept
    : nemesis::hkpAction(Class)
{
}

nemesis::hkpAction::hkpAction(const nemesis::hkClass& cls) noexcept
    : nemesis::HavokObject(cls.GetSignature())
    , nemesis::hkReferencedObject(cls)
{
}

void nemesis::hkpAction::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkReferencedObject::SerializeTo(serializer);       // offset: 0/0 size: 8/16 align: 4/8
    serializer.WriteSerializeIgnoredObject("world", m_world);   // offset: 8/16 size: 4/8 align: 4/8
    serializer.WriteSerializeIgnoredObject("island", m_island); // offset: 12/24 size: 4/8 align: 4/8
    serializer.WriteValue("userData", m_userData);              // offset: 16/32 size: 4/8 align: 4/8
    serializer.WriteValue("name", m_name);                      // offset: 20/40 size: 4/8 align: 4/8
    // class size: 24/48 align: 4/8
}

void nemesis::hkpAction::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    deserializer.ReadObject("world", m_world);                  // offset: 8/16 size: 4/8 align: 4/8
    deserializer.ReadObject("island", m_island);                // offset: 12/24 size: 4/8 align: 4/8
    deserializer.ReadValue("userData", m_userData);             // offset: 16/32 size: 4/8 align: 4/8
    deserializer.ReadValue("name", m_name);                     // offset: 20/40 size: 4/8 align: 4/8
    // class size: 24/48 align: 4/8
}
