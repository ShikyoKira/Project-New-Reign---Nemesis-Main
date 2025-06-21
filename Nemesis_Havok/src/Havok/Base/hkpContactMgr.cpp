#include "Havok/Base/hkpContactMgr.h"

nemesis::hkpContactMgr::hkpContactMgr(const nemesis::hkClass& cls)
    : nemesis::HavokObject(cls.GetSignature())
    , nemesis::hkReferencedObject(cls)
{
}

void nemesis::hkpContactMgr::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkReferencedObject::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8
    serializer.WriteValue("type", m_type);
    serializer.Pad(serializer.GetPointerSize());
    // class size: 8/16 align: 4/8
}

void nemesis::hkpContactMgr::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    deserializer.ReadValue("type", m_type);                     // offset: 8/16 size: 4/4 align: 4/4
    deserializer.Pad(deserializer.GetPointerSize());            // offset: 12/20 size: 0/4
    // class size: 12/24 align: 4/8
}
