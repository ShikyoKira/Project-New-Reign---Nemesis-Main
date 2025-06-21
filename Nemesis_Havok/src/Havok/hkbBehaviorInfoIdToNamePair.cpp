#include "Havok/hkbBehaviorInfoIdToNamePair.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbBehaviorInfoIdToNamePair,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbBehaviorInfoIdToNamePair::Class; });

nemesis::hkbBehaviorInfoIdToNamePair::hkbBehaviorInfoIdToNamePair() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkbBehaviorInfoIdToNamePair::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbBehaviorInfoIdToNamePair::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("behaviorName", m_behaviorName); // offset: 0/0 size: 4/8 align: 4/8
    serializer.WriteValue("nodeName", m_nodeName);         // offset: 4/8 size: 4/8 align: 4/8

    if (serializer.GetContentsVersion() > HavokVersion::HK_2011_3_0)
    {
        serializer.WriteValue("toolType", static_cast<unsigned short>(m_toolType.to_int()));
    }
    else
    {
        serializer.WriteValue("toolType", m_toolType); // offset: 8/16 size: 1/1 align: 1/1
        serializer.Skip(1);                            // offset: 9/17 size: 1/1
    }

    serializer.WriteValue("id", m_id);           // offset: 10/18 size: 2/2 align: 2/2
    serializer.Pad(serializer.GetPointerSize()); // offset: 12/20 size: 0/4
    // class size: 12/24 align: 4/8
}

void nemesis::hkbBehaviorInfoIdToNamePair::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("behaviorName", m_behaviorName); // offset: 0/0 size: 4/8 align: 4/8
    deserializer.ReadValue("nodeName", m_nodeName);         // offset: 4/8 size: 4/8 align: 4/8

    if (deserializer.GetContentsVersion() > HavokVersion::HK_2011_3_0)
    {
        unsigned short itoolType;
        deserializer.ReadValue("toolType", itoolType);
        m_toolType = static_cast<unsigned char>(itoolType);
    }
    else
    {
        deserializer.ReadValue("toolType", m_toolType); // offset: 8/16 size: 1/1 align: 1/1
        deserializer.Skip(1);                           // offset: 9/17 size: 1/1
    }

    deserializer.ReadValue("id", m_id);              // offset: 10/18 size: 2/2 align: 2/2
    deserializer.Pad(deserializer.GetPointerSize()); // offset: 12/20 size: 0/4
    // class size: 12/24 align: 4/8
}
