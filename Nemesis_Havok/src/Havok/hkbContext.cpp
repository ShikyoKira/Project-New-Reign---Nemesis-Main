#include "Havok/hkbContext.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbContext,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbContext::Class; });

nemesis::hkbContext::hkbContext() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkbContext::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbContext::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteSerializeIgnoredObject("character", m_character); // offset: 0/0 size: 4/8 align: 4/8

    auto cur_ver = serializer.GetContentsVersion();

    if (cur_ver > HavokVersion::HK_2011_3_0)
    {
        serializer.WriteSerializeIgnoredObject("rootBehavior", m_rootBehavior);
    }

    serializer.WriteSerializeIgnoredObject("behavior", m_behavior);   // offset: 4/8 size: 4/8 align: 4/8
    serializer.WriteSerializeIgnoredObject("nodeToIndexMap",
                                           m_nodeToIndexMap);           // offset: 8/16 size: 4/8 align: 4/8
    serializer.WriteSerializeIgnoredObject("eventQueue", m_eventQueue); // offset: 12/24 size: 4/8 align: 4/8
    serializer.WriteSerializeIgnoredObject("sharedEventQueue",
                                           m_sharedEventQueue); // offset: 16/32 size: 4/8 align: 4/8
    serializer.WriteObject("generatorOutputListener",
                           m_generatorOutputListener); // offset: 20/40 size: 4/8 align: 4/8

    // (Speculated memory layout portion)
    if (cur_ver <= HavokVersion::HK_710)
    {
        serializer.WriteObject("projectData", m_projectData);
    }

    serializer.WriteSerializeIgnoredValue("eventTriggeredTransition",
                                          m_eventTriggeredTransition); // offset: 24/48 size: 1/1 align: 1/1
    serializer.Pad(serializer.GetPointerSize());                       // offset: 25/49 size: 3/7

    serializer.WriteSerializeIgnoredObject("world", m_world); // offset: 28/56 size: 4/8 align: 4/8
    serializer.WriteSerializeIgnoredObject("attachmentManager",
                                           m_attachmentManager); // offset: 32/64 size: 4/8 align: 4/8
    serializer.WriteSerializeIgnoredObject("animationCache",
                                           m_animationCache); // offset: 36/72 size: 4/8 align: 4/8
    // class size: 40/80 align: 4/8
}

void nemesis::hkbContext::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadObject("character", m_character);               // offset: 0/0 size: 4/8 align: 4/8

    auto cur_ver = deserializer.GetContentsVersion();

    if (cur_ver > HavokVersion::HK_2011_3_0)
    {
        deserializer.ReadObject("rootBehavior", m_rootBehavior);
    }

    deserializer.ReadObject("behavior", m_behavior);                 // offset: 4/8 size: 4/8 align: 4/8
    deserializer.ReadObject("nodeToIndexMap", m_nodeToIndexMap);     // offset: 8/16 size: 4/8 align: 4/8
    deserializer.ReadObject("eventQueue", m_eventQueue);             // offset: 12/24 size: 4/8 align: 4/8
    deserializer.ReadObject("sharedEventQueue", m_sharedEventQueue); // offset: 16/32 size: 4/8 align: 4/8
    deserializer.ReadObject("generatorOutputListener",
                            m_generatorOutputListener); // offset: 20/40 size: 4/8 align: 4/8

    // (Speculated memory layout portion)
    if (cur_ver <= HavokVersion::HK_710)
    {
        deserializer.ReadObject("projectData", m_projectData);
    }

    deserializer.ReadValue("eventTriggeredTransition",
                           m_eventTriggeredTransition); // offset: 24/48 size: 1/1 align: 1/1
    deserializer.Pad(deserializer.GetPointerSize());    // offset: 25/49 size: 3/7

    deserializer.ReadObject("world", m_world);                         // offset: 28/56 size: 4/8 align: 4/8
    deserializer.ReadObject("attachmentManager", m_attachmentManager); // offset: 32/64 size: 4/8 align: 4/8
    deserializer.ReadObject("animationCache", m_animationCache);       // offset: 36/72 size: 4/8 align: 4/8
    // class size: 40/80 align: 4/8
}
