#include "Havok/BSTweenerModifier.h"

REGISTER_HAVOK_POINTER_SOURCE(BSTweenerModifier,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::BSTweenerModifier::Class; });

nemesis::BSTweenerModifier::BSTweenerModifier() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkbModifier(Class)
{
}

const nemesis::hkClass* nemesis::BSTweenerModifier::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::BSTweenerModifier::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkbModifier::SerializeTo(serializer);                 // offset: 0/0 size: 44/80 align: 4/8
    serializer.WriteValue("tweenPosition", m_tweenPosition);       // offset: 44/80 size: 1/1 align: 1/1
    serializer.WriteValue("tweenRotation", m_tweenRotation);       // offset: 45/81 size: 1/1 align: 1/1
    serializer.WriteValue("useTweenDuration", m_useTweenDuration); // offset: 46/82 size: 1/1 align: 1/1
    serializer.Skip(1);                                            // offset: 47/83 size: 1/1

    serializer.WriteValue("tweenDuration", m_tweenDuration); // offset: 48/84 size: 4/4 align: 4/4
    serializer.Pad(16);                                      // offset: 52/88 size: 12/8

    serializer.WriteValue("targetPosition", m_targetPosition);     // offset: 64/96 size: 16/16 align: 16/16
    serializer.WriteValue("targetRotation", m_targetRotation);     // offset: 80/112 size: 16/16 align: 16/16
    serializer.WriteSerializeIgnoredValue("duration", m_duration); // offset: 96/128 size: 4/4 align: 4/4
    serializer.Skip(12);                                           // offset: 100/132 size: 12/12

    serializer.WriteSerializeIgnoredValue("startTransform",
                                          m_startTransform); // offset: 112/144 size: 48/48 align: 16/16
    serializer.WriteSerializeIgnoredValue("time", m_time);   // offset: 160/192 size: 4/4 align: 4/4
    serializer.Skip(12);                                     // offset: 164/196 size: 12/12
    // class size: 176/208 align: 16/16
}

void nemesis::BSTweenerModifier::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkbModifier::DeserializeFrom(deserializer);            // offset: 0/0 size: 44/80 align: 4/8
    deserializer.ReadValue("tweenPosition", m_tweenPosition);       // offset: 44/80 size: 1/1 align: 1/1
    deserializer.ReadValue("tweenRotation", m_tweenRotation);       // offset: 45/81 size: 1/1 align: 1/1
    deserializer.ReadValue("useTweenDuration", m_useTweenDuration); // offset: 46/82 size: 1/1 align: 1/1
    deserializer.Skip(1);                                           // offset: 47/83 size: 1/1

    deserializer.ReadValue("tweenDuration", m_tweenDuration); // offset: 48/84 size: 4/4 align: 4/4
    deserializer.Pad(16);                                     // offset: 52/88 size: 12/8

    deserializer.ReadValue("targetPosition", m_targetPosition); // offset: 64/96 size: 16/16 align: 16/16
    deserializer.ReadValue("targetRotation", m_targetRotation); // offset: 80/112 size: 16/16 align: 16/16
    deserializer.ReadValue("duration", m_duration);             // offset: 96/128 size: 4/4 align: 4/4
    deserializer.Skip(12);                                      // offset: 100/132 size: 12/12

    deserializer.ReadValue("startTransform", m_startTransform); // offset: 112/144 size: 48/48 align: 16/16
    deserializer.ReadValue("time", m_time);                     // offset: 160/192 size: 4/4 align: 4/4
    deserializer.Skip(12);                                      // offset: 164/196 size: 12/12
    // class size: 176/208 align: 16/16
}
