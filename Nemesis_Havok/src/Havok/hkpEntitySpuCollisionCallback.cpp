#include "Havok/hkpEntitySpuCollisionCallback.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpEntitySpuCollisionCallback,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpEntitySpuCollisionCallback::Class; });

nemesis::hkpEntitySpuCollisionCallback::hkpEntitySpuCollisionCallback() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkpEntitySpuCollisionCallback::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpEntitySpuCollisionCallback::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteSerializeIgnoredObject("util", m_util);        // offset: 0/0 size: 4/8 align: 4/8
    serializer.WriteSerializeIgnoredValue("capacity", m_capacity); // offset: 4/8 size: 2/2 align: 2/2
    serializer.WriteValue("eventFilter", m_eventFilter);           // offset: 6/10 size: 1/1 align: 1/1
    serializer.WriteValue("userFilter", m_userFilter);             // offset: 7/11 size: 1/1 align: 1/1
    serializer.Pad(serializer.GetPointerSize());                   // offset: 8/12 size: 0/4
    // class size: 8/16 align: 4/8
}

void nemesis::hkpEntitySpuCollisionCallback::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadObject("util", m_util);              // offset: 0/0 size: 4/8 align: 4/8
    deserializer.ReadValue("capacity", m_capacity);       // offset: 4/8 size: 2/2 align: 2/2
    deserializer.ReadValue("eventFilter", m_eventFilter); // offset: 6/10 size: 1/1 align: 1/1
    deserializer.ReadValue("userFilter", m_userFilter);   // offset: 7/11 size: 1/1 align: 1/1
    deserializer.Pad(deserializer.GetPointerSize());      // offset: 8/12 size: 0/4
    // class size: 8/16 align: 4/8
}
