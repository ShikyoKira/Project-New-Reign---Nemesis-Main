#include "Havok/hkaQuantizedAnimation.h"

REGISTER_HAVOK_POINTER_SOURCE(hkaQuantizedAnimation,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkaQuantizedAnimation::Class; });

nemesis::hkaQuantizedAnimation::hkaQuantizedAnimation() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkaAnimation(Class)
{
}

const nemesis::hkClass* nemesis::hkaQuantizedAnimation::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkaQuantizedAnimation::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkaAnimation::SerializeTo(serializer); // offset: 0/0 size: 40/56 align: 4/8
    serializer.WriteObject("data", m_data);         // offset: 40/56 size: 12/16 align: 4/8
    serializer.WriteValue("endian", m_endian);      // offset: 52/72 size: 4/4 align: 4/4
    serializer.Pad(serializer.GetPointerSize());    // offset: 56/76 size: 0/4

    serializer.WriteSerializeIgnoredObject("skeleton", m_skeleton); // offset: 56/80 size: 4/8 align: 4/8
    // class size: 60/88 align: 4/8
}

void nemesis::hkaQuantizedAnimation::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkaAnimation::DeserializeFrom(deserializer); // offset: 0/0 size: 40/56 align: 4/8
    deserializer.ReadObject("data", m_data);              // offset: 40/56 size: 12/16 align: 4/8
    deserializer.ReadValue("endian", m_endian);           // offset: 52/72 size: 4/4 align: 4/4
    deserializer.Pad(deserializer.GetPointerSize());      // offset: 56/76 size: 0/4

    deserializer.ReadObject("skeleton", m_skeleton); // offset: 56/80 size: 4/8 align: 4/8
    // class size: 60/88 align: 4/8
}
