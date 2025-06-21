#include "Havok/Base/hkpWorldObject.h"

nemesis::hkpWorldObject::hkpWorldObject(const nemesis::hkClass& cls)
    : nemesis::HavokObject(cls.GetSignature())
    , nemesis::hkReferencedObject(cls)
{
}

void nemesis::hkpWorldObject::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkReferencedObject::SerializeTo(serializer);           // offset: 0/0 size: 8/16 align: 4/8
    serializer.WriteSerializeIgnoredObject("world", m_world);       // offset: 8/16 size: 4/8 align: 4/8
    serializer.WriteValue("userData", m_userData);                  // offset: 12/24 size: 4/8 align: 4/8
    serializer.WriteObject("collidable", m_collidable);             // offset: 16/32 size: 92/128 align: 4/8
    serializer.WriteObject("multiThreadCheck", m_multiThreadCheck); // offset: 108/160 size: 12/12 align: 4/4
    serializer.Pad(serializer.GetPointerSize());                    // offset: 120/172 size: 0/4

    serializer.WriteValue("name", m_name);                // offset: 120/176 size: 4/8 align: 4/8
    serializer.WriteObject("properties", m_properties_0); // offset: 124/184 size: 12/16 align: 4/8

    if (serializer.GetContentsVersion() <= HavokVersion::HK_2010_2_0)
    {
        serializer.WriteSerializeIgnoredObject("treeData",
                                               m_treeData); // offset: 136/200 size: 4/8 align: 4/8
    }

    // class size: 140/208 align: 4/8
}

void nemesis::hkpWorldObject::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkReferencedObject::DeserializeFrom(deserializer);      // offset: 0/0 size: 8/16 align: 4/8
    deserializer.ReadObject("world", m_world);                       // offset: 8/16 size: 4/8 align: 4/8
    deserializer.ReadValue("userData", m_userData);                  // offset: 12/24 size: 4/8 align: 4/8
    deserializer.ReadObject("collidable", m_collidable);             // offset: 16/32 size: 92/128 align: 4/8
    deserializer.ReadObject("multiThreadCheck", m_multiThreadCheck); // offset: 108/160 size: 12/12 align: 4/4
    deserializer.Pad(deserializer.GetPointerSize());                 // offset: 120/172 size: 0/4

    deserializer.ReadValue("name", m_name);                // offset: 120/176 size: 4/8 align: 4/8
    deserializer.ReadObject("properties", m_properties_0); // offset: 124/184 size: 12/16 align: 4/8

    if (deserializer.GetContentsVersion() <= HavokVersion::HK_2010_2_0)
    {
        deserializer.ReadObject("treeData", m_treeData); // offset: 136/200 size: 4/8 align: 4/8
    }

    // class size: 140/208 align: 4/8
}
