#include "Havok/hkbBlenderGenerator.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbBlenderGenerator,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbBlenderGenerator::Class; });

nemesis::hkbBlenderGenerator::hkbBlenderGenerator() noexcept
    : nemesis::hkbBlenderGenerator(Class)
{
}

nemesis::hkbBlenderGenerator::hkbBlenderGenerator(const nemesis::hkClass& cls) noexcept
    : nemesis::HavokObject(cls.GetSignature())
    , nemesis::hkbGenerator(cls)
{
}

const nemesis::hkClass* nemesis::hkbBlenderGenerator::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbBlenderGenerator::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkbGenerator::SerializeTo(serializer); // offset: 0/0 size: 40/72 align: 4/8
    serializer.WriteValue("referencePoseWeightThreshold",
                          m_referencePoseWeightThreshold);     // offset: 40/72 size: 4/4 align: 4/4
    serializer.WriteValue("blendParameter", m_blendParameter); // offset: 44/76 size: 4/4 align: 4/4
    serializer.WriteValue("minCyclicBlendParameter",
                          m_minCyclicBlendParameter); // offset: 48/80 size: 4/4 align: 4/4
    serializer.WriteValue("maxCyclicBlendParameter",
                          m_maxCyclicBlendParameter); // offset: 52/84 size: 4/4 align: 4/4
    serializer.WriteValue("indexOfSyncMasterChild",
                          m_indexOfSyncMasterChild);                 // offset: 56/88 size: 2/2 align: 2/2
    serializer.WriteValue("flags", m_flags);                         // offset: 58/90 size: 2/2 align: 2/2
    serializer.WriteValue("subtractLastChild", m_subtractLastChild); // offset: 60/92 size: 1/1 align: 1/1
    serializer.Pad(serializer.GetPointerSize());                     // offset: 61/93 size: 3/3

    serializer.WriteObject("children", m_children); // offset: 64/96 size: 12/16 align: 4/8
    serializer.WriteSerializeIgnoredObject("childrenInternalStates",
                                           m_childrenInternalStates); // offset: 76/112 size: 12/16 align: 4/8
    serializer.WriteSerializeIgnoredObject("sortedChildren",
                                           m_sortedChildren); // offset: 88/128 size: 12/16 align: 4/8
    serializer.WriteSerializeIgnoredValue("endIntervalWeight",
                                          m_endIntervalWeight); // offset: 100/144 size: 4/4 align: 4/4
    serializer.WriteSerializeIgnoredValue("numActiveChildren",
                                          m_numActiveChildren); // offset: 104/148 size: 4/4 align: 4/4
    serializer.WriteSerializeIgnoredValue("beginIntervalIndex",
                                          m_beginIntervalIndex); // offset: 108/152 size: 2/2 align: 2/2
    serializer.WriteSerializeIgnoredValue("endIntervalIndex",
                                          m_endIntervalIndex);     // offset: 110/154 size: 2/2 align: 2/2
    serializer.WriteSerializeIgnoredValue("initSync", m_initSync); // offset: 112/156 size: 1/1 align: 1/1
    serializer.WriteSerializeIgnoredValue("doSubtractiveBlend",
                                          m_doSubtractiveBlend); // offset: 113/157 size: 1/1 align: 1/1
    serializer.Skip(2);                                          // offset: 114/158 size: 2/2
    // class size: 116/160 align: 4/8
}

void nemesis::hkbBlenderGenerator::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkbGenerator::DeserializeFrom(deserializer); // offset: 0/0 size: 40/72 align: 4/8
    deserializer.ReadValue("referencePoseWeightThreshold",
                           m_referencePoseWeightThreshold);     // offset: 40/72 size: 4/4 align: 4/4
    deserializer.ReadValue("blendParameter", m_blendParameter); // offset: 44/76 size: 4/4 align: 4/4
    deserializer.ReadValue("minCyclicBlendParameter",
                           m_minCyclicBlendParameter); // offset: 48/80 size: 4/4 align: 4/4
    deserializer.ReadValue("maxCyclicBlendParameter",
                           m_maxCyclicBlendParameter); // offset: 52/84 size: 4/4 align: 4/4
    deserializer.ReadValue("indexOfSyncMasterChild",
                           m_indexOfSyncMasterChild);                 // offset: 56/88 size: 2/2 align: 2/2
    deserializer.ReadValue("flags", m_flags);                         // offset: 58/90 size: 2/2 align: 2/2
    deserializer.ReadValue("subtractLastChild", m_subtractLastChild); // offset: 60/92 size: 1/1 align: 1/1
    deserializer.Pad(deserializer.GetPointerSize());                  // offset: 61/93 size: 3/3

    deserializer.ReadObject("children", m_children); // offset: 64/96 size: 12/16 align: 4/8
    deserializer.ReadObject("childrenInternalStates",
                            m_childrenInternalStates);                // offset: 76/112 size: 12/16 align: 4/8
    deserializer.ReadObject("sortedChildren", m_sortedChildren);      // offset: 88/128 size: 12/16 align: 4/8
    deserializer.ReadValue("endIntervalWeight", m_endIntervalWeight); // offset: 100/144 size: 4/4 align: 4/4
    deserializer.ReadValue("numActiveChildren", m_numActiveChildren); // offset: 104/148 size: 4/4 align: 4/4
    deserializer.ReadValue("beginIntervalIndex",
                           m_beginIntervalIndex);                   // offset: 108/152 size: 2/2 align: 2/2
    deserializer.ReadValue("endIntervalIndex", m_endIntervalIndex); // offset: 110/154 size: 2/2 align: 2/2
    deserializer.ReadValue("initSync", m_initSync);                 // offset: 112/156 size: 1/1 align: 1/1
    deserializer.ReadValue("doSubtractiveBlend",
                           m_doSubtractiveBlend); // offset: 113/157 size: 1/1 align: 1/1
    deserializer.Skip(2);                         // offset: 114/158 size: 2/2
    // class size: 116/160 align: 4/8
}
