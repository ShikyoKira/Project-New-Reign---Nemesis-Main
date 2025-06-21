#include "Havok/hkpMouseSpringAction.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpMouseSpringAction,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpMouseSpringAction::Class; });

nemesis::hkpMouseSpringAction::hkpMouseSpringAction() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkpUnaryAction(Class)
{
}

const nemesis::hkClass* nemesis::hkpMouseSpringAction::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpMouseSpringAction::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpUnaryAction::SerializeTo(serializer); // offset: 0/0 size: 28/56 align: 4/8
    serializer.Pad(16);                               // offset: 28/56 size: 4/8

    serializer.WriteValue("positionInRbLocal", m_positionInRbLocal); // offset: 32/64 size: 16/16 align: 16/16
    serializer.WriteValue("mousePositionInWorld",
                          m_mousePositionInWorld);                 // offset: 48/80 size: 16/16 align: 16/16
    serializer.WriteValue("springDamping", m_springDamping);       // offset: 64/96 size: 4/4 align: 4/4
    serializer.WriteValue("springElasticity", m_springElasticity); // offset: 68/100 size: 4/4 align: 4/4
    serializer.WriteValue("maxRelativeForce", m_maxRelativeForce); // offset: 72/104 size: 4/4 align: 4/4
    serializer.WriteValue("objectDamping", m_objectDamping);       // offset: 76/108 size: 4/4 align: 4/4
    serializer.WriteValue("shapeKey", m_shapeKey);                 // offset: 80/112 size: 4/4 align: 4/4
    serializer.Pad(serializer.GetPointerSize());                   // offset: 84/116 size: 0/4

    serializer.WriteSerializeIgnoredObject("applyCallbacks",
                                           m_applyCallbacks); // offset: 84/120 size: 12/16 align: 4/8
    serializer.Pad(16);                                       // offset: 96/136 size: 0/8
    // class size: 96/144 align: 16/16
}

void nemesis::hkpMouseSpringAction::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpUnaryAction::DeserializeFrom(deserializer); // offset: 0/0 size: 28/56 align: 4/8
    deserializer.Pad(16);                                   // offset: 28/56 size: 4/8

    deserializer.ReadValue("positionInRbLocal",
                           m_positionInRbLocal); // offset: 32/64 size: 16/16 align: 16/16
    deserializer.ReadValue("mousePositionInWorld",
                           m_mousePositionInWorld);                 // offset: 48/80 size: 16/16 align: 16/16
    deserializer.ReadValue("springDamping", m_springDamping);       // offset: 64/96 size: 4/4 align: 4/4
    deserializer.ReadValue("springElasticity", m_springElasticity); // offset: 68/100 size: 4/4 align: 4/4
    deserializer.ReadValue("maxRelativeForce", m_maxRelativeForce); // offset: 72/104 size: 4/4 align: 4/4
    deserializer.ReadValue("objectDamping", m_objectDamping);       // offset: 76/108 size: 4/4 align: 4/4
    deserializer.ReadValue("shapeKey", m_shapeKey);                 // offset: 80/112 size: 4/4 align: 4/4
    deserializer.Pad(deserializer.GetPointerSize());                // offset: 84/116 size: 0/4

    deserializer.ReadObject("applyCallbacks", m_applyCallbacks); // offset: 84/120 size: 12/16 align: 4/8
    deserializer.Pad(16);                                        // offset: 96/136 size: 0/8
    // class size: 96/144 align: 16/16
}
