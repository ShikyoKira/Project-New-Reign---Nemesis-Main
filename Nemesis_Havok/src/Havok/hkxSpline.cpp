#include "Havok/hkxSpline.h"

REGISTER_HAVOK_POINTER_SOURCE(hkxSpline,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkxSpline::Class; });

nemesis::hkxSpline::hkxSpline() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkxSpline::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkxSpline::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkReferencedObject::SerializeTo(serializer);     // offset: 0/0 size: 8/16 align: 4/8
    serializer.WriteObject("controlPoints", m_controlPoints); // offset: 8/16 size: 12/16 align: 4/8
    serializer.WriteValue("isClosed", m_isClosed);            // offset: 20/32 size: 1/1 align: 1/1
    serializer.Pad(serializer.GetPointerSize());              // offset: 21/33 size: 3/7
    // class size: 24/40 align: 4/8
}

void nemesis::hkxSpline::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    deserializer.ReadObject("controlPoints", m_controlPoints);  // offset: 8/16 size: 12/16 align: 4/8
    deserializer.ReadValue("isClosed", m_isClosed);             // offset: 20/32 size: 1/1 align: 1/1
    deserializer.Pad(deserializer.GetPointerSize());            // offset: 21/33 size: 3/7
    // class size: 24/40 align: 4/8
}
