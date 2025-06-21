#include "Havok/hkaSkeleton.h"

REGISTER_HAVOK_POINTER_SOURCE(hkaSkeleton,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkaSkeleton::Class; });

nemesis::hkaSkeleton::hkaSkeleton() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkaSkeleton::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkaSkeleton::SerializeTo(nemesis::Serializer& serializer) const
{
    auto cur_ver = serializer.GetContentsVersion();

    if (cur_ver > HavokVersion::HK_660)
    {
        nemesis::hkReferencedObject::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8
    }

    serializer.WriteValue("name", m_name);                        // offset: 8/16 size: 4/8 align: 4/8
    serializer.WriteObject("parentIndices", m_parentIndices);     // offset: 12/24 size: 12/16 align: 4/8
    serializer.WriteObject("bones", m_bones);                     // offset: 24/40 size: 12/16 align: 4/8
    serializer.WriteObject("referencePose", m_referencePose);     // offset: 36/56 size: 12/16 align: 4/8

    if (cur_ver > HavokVersion::HK_710)
    {
        serializer.WriteObject("referenceFloats", m_referenceFloats); // offset: 48/72 size: 12/16 align: 4/8
    }

    serializer.WriteObject("floatSlots", m_floatSlots);           // offset: 60/88 size: 12/16 align: 4/8
    serializer.WriteObject("localFrames", m_localFrames);         // offset: 72/104 size: 12/16 align: 4/8

    if (cur_ver > HavokVersion::HK_2011_3_0)
    {
        serializer.WriteObject("partitions", m_partitions);
    }
    
    // class size: 84/120 align: 4/8
}

void nemesis::hkaSkeleton::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    auto cur_ver = deserializer.GetContentsVersion();

    if (cur_ver > HavokVersion::HK_660)
    {
        nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    }

    deserializer.ReadValue("name", m_name);                        // offset: 8/16 size: 4/8 align: 4/8
    deserializer.ReadObject("parentIndices", m_parentIndices);     // offset: 12/24 size: 12/16 align: 4/8
    deserializer.ReadObject("bones", m_bones);                     // offset: 24/40 size: 12/16 align: 4/8
    deserializer.ReadObject("referencePose", m_referencePose);     // offset: 36/56 size: 12/16 align: 4/8

    if (cur_ver > HavokVersion::HK_710)
    {
        deserializer.ReadObject("referenceFloats", m_referenceFloats); // offset: 48/72 size: 12/16 align: 4/8
    }

    deserializer.ReadObject("floatSlots", m_floatSlots);           // offset: 60/88 size: 12/16 align: 4/8
    deserializer.ReadObject("localFrames", m_localFrames);         // offset: 72/104 size: 12/16 align: 4/8

    if (cur_ver > HavokVersion::HK_2011_3_0)
    {
        deserializer.ReadObject("partitions", m_partitions);
    }

    // class size: 84/120 align: 4/8
}
