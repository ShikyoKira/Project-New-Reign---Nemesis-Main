#include "Havok/hkbBehaviorInfo.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbBehaviorInfo,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbBehaviorInfo::Class; });

nemesis::hkbBehaviorInfo::hkbBehaviorInfo() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkbBehaviorInfo::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbBehaviorInfo::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkReferencedObject::SerializeTo(serializer);     // offset: 0/0 size: 8/16 align: 4/8
    serializer.WriteValue("characterId", m_characterId);      // offset: 8/16 size: 8/8 align: 8/8
    serializer.WriteObject("data", m_data);                   // offset: 16/24 size: 4/8 align: 4/8

    if (serializer.GetContentsVersion() > HavokVersion::HK_2010_1_0)
    {
        serializer.WriteObject("idToNamePairs", m_idToNamePairs); // offset: 20/32 size: 12/16 align: 4/8
    }
    else
    {
        serializer.Pad(8);
    }

    // class size: 32/48 align: 8/8
}

void nemesis::hkbBehaviorInfo::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    deserializer.ReadValue("characterId", m_characterId);       // offset: 8/16 size: 8/8 align: 8/8
    deserializer.ReadObject("data", m_data);                    // offset: 16/24 size: 4/8 align: 4/8

    if (deserializer.GetContentsVersion() > HavokVersion::HK_2010_1_0)
    {
        deserializer.ReadObject("idToNamePairs", m_idToNamePairs); // offset: 20/32 size: 12/16 align: 4/8
    }
    else
    {
        deserializer.Pad(8);
    }

    // class size: 32/48 align: 8/8
}
