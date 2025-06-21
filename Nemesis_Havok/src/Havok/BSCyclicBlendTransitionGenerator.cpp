#include "Havok/BSCyclicBlendTransitionGenerator.h"

REGISTER_HAVOK_POINTER_SOURCE(BSCyclicBlendTransitionGenerator,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::BSCyclicBlendTransitionGenerator::Class; });

nemesis::BSCyclicBlendTransitionGenerator::BSCyclicBlendTransitionGenerator() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkbGenerator(Class)
{
}

const nemesis::hkClass* nemesis::BSCyclicBlendTransitionGenerator::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::BSCyclicBlendTransitionGenerator::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkbGenerator::SerializeTo(serializer); // offset: 0/0 size: 40/72 align: 4/8
    serializer.Pad(16);                             // offset: 40/72 size: 8/8

    serializer.WriteObject("pBlenderGenerator", m_pBlenderGenerator); // offset: 48/80 size: 4/8 align: 16/16
    serializer.WriteObject("EventToFreezeBlendValue",
                           m_EventToFreezeBlendValue);                // offset: 52/88 size: 8/16 align: 4/8
    serializer.WriteObject("EventToCrossBlend", m_EventToCrossBlend); // offset: 60/104 size: 8/16 align: 4/8
    serializer.WriteValue("fBlendParameter", m_fBlendParameter);      // offset: 68/120 size: 4/4 align: 4/4
    serializer.WriteValue("fTransitionDuration",
                          m_fTransitionDuration);        // offset: 72/124 size: 4/4 align: 4/4
    serializer.WriteValue("eBlendCurve", m_eBlendCurve); // offset: 76/128 size: 1/1 align: 1/1
    serializer.Pad(16);                                  // offset: 77/129 size: 3/15

    serializer.WriteSerializeIgnoredObject(
        "pTransitionBlenderGenerator",
        m_pTransitionBlenderGenerator); // offset: 80/144 size: 4/8 align: 16/16
    serializer.Pad(16);                 // offset: 84/152 size: 12/8

    serializer.WriteSerializeIgnoredObject("pTransitionEffect",
                                           m_pTransitionEffect); // offset: 96/160 size: 4/8 align: 16/16
    serializer.WriteSerializeIgnoredValue("currentMode",
                                          m_currentMode); // offset: 100/168 size: 1/1 align: 1/1
    serializer.Pad(16);                                   // offset: 101/169 size: 11/7
    // class size: 112/176 align: 16/16
}

void nemesis::BSCyclicBlendTransitionGenerator::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkbGenerator::DeserializeFrom(deserializer); // offset: 0/0 size: 40/72 align: 4/8
    deserializer.Pad(16);                                 // offset: 40/72 size: 8/8

    deserializer.ReadObject("pBlenderGenerator", m_pBlenderGenerator); // offset: 48/80 size: 4/8 align: 16/16
    deserializer.ReadObject("EventToFreezeBlendValue",
                            m_EventToFreezeBlendValue);                // offset: 52/88 size: 8/16 align: 4/8
    deserializer.ReadObject("EventToCrossBlend", m_EventToCrossBlend); // offset: 60/104 size: 8/16 align: 4/8
    deserializer.ReadValue("fBlendParameter", m_fBlendParameter);      // offset: 68/120 size: 4/4 align: 4/4
    deserializer.ReadValue("fTransitionDuration",
                           m_fTransitionDuration);        // offset: 72/124 size: 4/4 align: 4/4
    deserializer.ReadValue("eBlendCurve", m_eBlendCurve); // offset: 76/128 size: 1/1 align: 1/1
    deserializer.Pad(16);                                 // offset: 77/129 size: 3/15

    deserializer.ReadObject("pTransitionBlenderGenerator",
                            m_pTransitionBlenderGenerator); // offset: 80/144 size: 4/8 align: 16/16
    deserializer.Pad(16);                                   // offset: 84/152 size: 12/8

    deserializer.ReadObject("pTransitionEffect",
                            m_pTransitionEffect);         // offset: 96/160 size: 4/8 align: 16/16
    deserializer.ReadValue("currentMode", m_currentMode); // offset: 100/168 size: 1/1 align: 1/1
    deserializer.Pad(16);                                 // offset: 101/169 size: 11/7
    // class size: 112/176 align: 16/16
}
