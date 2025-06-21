#include "Havok/hkbCameraShakeEventPayload.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbCameraShakeEventPayload,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbCameraShakeEventPayload::Class; });

nemesis::hkbCameraShakeEventPayload::hkbCameraShakeEventPayload() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkbEventPayload(Class)
{
}

const nemesis::hkClass* nemesis::hkbCameraShakeEventPayload::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbCameraShakeEventPayload::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkbEventPayload::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8
    serializer.WriteValue("amplitude", m_amplitude);   // offset: 8/16 size: 4/4 align: 4/4
    serializer.WriteValue("halfLife", m_halfLife);     // offset: 12/20 size: 4/4 align: 4/4
    // class size: 16/24 align: 4/8
}

void nemesis::hkbCameraShakeEventPayload::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkbEventPayload::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    deserializer.ReadValue("amplitude", m_amplitude);        // offset: 8/16 size: 4/4 align: 4/4
    deserializer.ReadValue("halfLife", m_halfLife);          // offset: 12/20 size: 4/4 align: 4/4
    // class size: 16/24 align: 4/8
}
