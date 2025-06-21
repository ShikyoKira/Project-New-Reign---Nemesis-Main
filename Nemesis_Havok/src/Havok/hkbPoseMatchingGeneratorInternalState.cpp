#include "Havok/hkbPoseMatchingGeneratorInternalState.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbPoseMatchingGeneratorInternalState,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbPoseMatchingGeneratorInternalState::Class; });

nemesis::hkbPoseMatchingGeneratorInternalState::hkbPoseMatchingGeneratorInternalState() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkbPoseMatchingGeneratorInternalState::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbPoseMatchingGeneratorInternalState::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkReferencedObject::SerializeTo(serializer);  // offset: 0/0 size: 8/16 align: 4/8
    serializer.WriteValue("currentMatch", m_currentMatch); // offset: 8/16 size: 4/4 align: 4/4
    serializer.WriteValue("bestMatch", m_bestMatch);       // offset: 12/20 size: 4/4 align: 4/4
    serializer.WriteValue("timeSinceBetterMatch",
                          m_timeSinceBetterMatch); // offset: 16/24 size: 4/4 align: 4/4
    serializer.WriteValue("error", m_error);       // offset: 20/28 size: 4/4 align: 4/4
    serializer.WriteValue("resetCurrentMatchLocalTime",
                          m_resetCurrentMatchLocalTime); // offset: 24/32 size: 1/1 align: 1/1
    serializer.Pad(serializer.GetPointerSize());         // offset: 25/33 size: 3/7
    // class size: 28/40 align: 4/8
}

void nemesis::hkbPoseMatchingGeneratorInternalState::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    deserializer.ReadValue("currentMatch", m_currentMatch);     // offset: 8/16 size: 4/4 align: 4/4
    deserializer.ReadValue("bestMatch", m_bestMatch);           // offset: 12/20 size: 4/4 align: 4/4
    deserializer.ReadValue("timeSinceBetterMatch",
                           m_timeSinceBetterMatch); // offset: 16/24 size: 4/4 align: 4/4
    deserializer.ReadValue("error", m_error);       // offset: 20/28 size: 4/4 align: 4/4
    deserializer.ReadValue("resetCurrentMatchLocalTime",
                           m_resetCurrentMatchLocalTime); // offset: 24/32 size: 1/1 align: 1/1
    deserializer.Pad(deserializer.GetPointerSize());      // offset: 25/33 size: 3/7
    // class size: 28/40 align: 4/8
}
