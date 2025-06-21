#include "Havok/BSEventOnFalseToTrueModifier.h"

REGISTER_HAVOK_POINTER_SOURCE(BSEventOnFalseToTrueModifier,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::BSEventOnFalseToTrueModifier::Class; });

nemesis::BSEventOnFalseToTrueModifier::BSEventOnFalseToTrueModifier() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkbModifier(Class)
{
}

const nemesis::hkClass* nemesis::BSEventOnFalseToTrueModifier::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::BSEventOnFalseToTrueModifier::SerializeTo(nemesis::Serializer& serializer) const
{
    unsigned int padding_size = serializer.GetPointerSize();

    nemesis::hkbModifier::SerializeTo(serializer);                 // offset: 0/0 size: 44/80 align: 4/8
    serializer.WriteValue("bEnableEvent1", m_bEnableEvent1);       // offset: 44/80 size: 1/1 align: 1/1
    serializer.WriteValue("bVariableToTest1", m_bVariableToTest1); // offset: 45/81 size: 1/1 align: 1/1
    serializer.Pad(padding_size);                                  // offset: 46/82 size: 2/6

    serializer.WriteObject("EventToSend1", m_EventToSend1);        // offset: 48/88 size: 8/16 align: 4/8
    serializer.WriteValue("bEnableEvent2", m_bEnableEvent2);       // offset: 56/104 size: 1/1 align: 1/1
    serializer.WriteValue("bVariableToTest2", m_bVariableToTest2); // offset: 57/105 size: 1/1 align: 1/1
    serializer.Pad(padding_size);                                  // offset: 58/106 size: 2/6

    serializer.WriteObject("EventToSend2", m_EventToSend2);        // offset: 60/112 size: 8/16 align: 4/8
    serializer.WriteValue("bEnableEvent3", m_bEnableEvent3);       // offset: 68/128 size: 1/1 align: 1/1
    serializer.WriteValue("bVariableToTest3", m_bVariableToTest3); // offset: 69/129 size: 1/1 align: 1/1
    serializer.Pad(padding_size);                                  // offset: 70/130 size: 2/6

    serializer.WriteObject("EventToSend3", m_EventToSend3); // offset: 72/136 size: 8/16 align: 4/8
    serializer.WriteSerializeIgnoredValue("bSlot1ActivatedLastFrame",
                                          m_bSlot1ActivatedLastFrame); // offset: 80/152 size: 1/1 align: 1/1
    serializer.WriteSerializeIgnoredValue("bSlot2ActivatedLastFrame",
                                          m_bSlot2ActivatedLastFrame); // offset: 81/153 size: 1/1 align: 1/1
    serializer.WriteSerializeIgnoredValue("bSlot3ActivatedLastFrame",
                                          m_bSlot3ActivatedLastFrame); // offset: 82/154 size: 1/1 align: 1/1
    serializer.Pad(padding_size);                                      // offset: 83/155 size: 1/5
    // class size: 84/160 align: 4/8
}

void nemesis::BSEventOnFalseToTrueModifier::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    unsigned int padding_size = deserializer.GetPointerSize();

    nemesis::hkbModifier::DeserializeFrom(deserializer);            // offset: 0/0 size: 44/80 align: 4/8
    deserializer.ReadValue("bEnableEvent1", m_bEnableEvent1);       // offset: 44/80 size: 1/1 align: 1/1
    deserializer.ReadValue("bVariableToTest1", m_bVariableToTest1); // offset: 45/81 size: 1/1 align: 1/1
    deserializer.Pad(padding_size);                                 // offset: 46/82 size: 2/6

    deserializer.ReadObject("EventToSend1", m_EventToSend1);        // offset: 48/88 size: 8/16 align: 4/8
    deserializer.ReadValue("bEnableEvent2", m_bEnableEvent2);       // offset: 56/104 size: 1/1 align: 1/1
    deserializer.ReadValue("bVariableToTest2", m_bVariableToTest2); // offset: 57/105 size: 1/1 align: 1/1
    deserializer.Pad(padding_size);                                 // offset: 58/106 size: 2/6

    deserializer.ReadObject("EventToSend2", m_EventToSend2);        // offset: 60/112 size: 8/16 align: 4/8
    deserializer.ReadValue("bEnableEvent3", m_bEnableEvent3);       // offset: 68/128 size: 1/1 align: 1/1
    deserializer.ReadValue("bVariableToTest3", m_bVariableToTest3); // offset: 69/129 size: 1/1 align: 1/1
    deserializer.Pad(padding_size);                                 // offset: 70/130 size: 2/6

    deserializer.ReadObject("EventToSend3", m_EventToSend3); // offset: 72/136 size: 8/16 align: 4/8
    deserializer.ReadValue("bSlot1ActivatedLastFrame",
                           m_bSlot1ActivatedLastFrame); // offset: 80/152 size: 1/1 align: 1/1
    deserializer.ReadValue("bSlot2ActivatedLastFrame",
                           m_bSlot2ActivatedLastFrame); // offset: 81/153 size: 1/1 align: 1/1
    deserializer.ReadValue("bSlot3ActivatedLastFrame",
                           m_bSlot3ActivatedLastFrame); // offset: 82/154 size: 1/1 align: 1/1
    deserializer.Pad(padding_size);                     // offset: 83/155 size: 1/5
    // class size: 84/160 align: 4/8
}
