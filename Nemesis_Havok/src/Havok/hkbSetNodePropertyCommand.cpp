#include "Havok/hkbSetNodePropertyCommand.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbSetNodePropertyCommand,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbSetNodePropertyCommand::Class; });

nemesis::hkbSetNodePropertyCommand::hkbSetNodePropertyCommand() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkbSetNodePropertyCommand::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbSetNodePropertyCommand::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkReferencedObject::SerializeTo(serializer);     // offset: 0/0 size: 8/16 align: 4/8
    serializer.WriteValue("characterId", m_characterId);      // offset: 8/16 size: 8/8 align: 8/8
    serializer.WriteValue("nodeName", m_nodeName);            // offset: 16/24 size: 4/8 align: 4/8
    serializer.WriteValue("propertyName", m_propertyName);    // offset: 20/32 size: 4/8 align: 4/8
    serializer.WriteObject("propertyValue", m_propertyValue); // offset: 24/40 size: 4/4 align: 4/4

    if (serializer.GetContentsVersion() > HavokVersion::HK_2010_1_0)
    {
        serializer.WriteValue("padding", m_padding); // offset: 28/44 size: 4/4 align: 4/4
    }

    // class size: 32/48 align: 8/8
}

void nemesis::hkbSetNodePropertyCommand::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    deserializer.ReadValue("characterId", m_characterId);       // offset: 8/16 size: 8/8 align: 8/8
    deserializer.ReadValue("nodeName", m_nodeName);             // offset: 16/24 size: 4/8 align: 4/8
    deserializer.ReadValue("propertyName", m_propertyName);     // offset: 20/32 size: 4/8 align: 4/8
    deserializer.ReadObject("propertyValue", m_propertyValue);  // offset: 24/40 size: 4/4 align: 4/4

    if (deserializer.GetContentsVersion() > HavokVersion::HK_2010_1_0)
    {
        deserializer.ReadValue("padding", m_padding); // offset: 28/44 size: 4/4 align: 4/4
    }

    // class size: 32/48 align: 8/8
}
