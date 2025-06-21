#include "Havok/hkbBlendingTransitionEffectInternalState.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbBlendingTransitionEffectInternalState,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbBlendingTransitionEffectInternalState::Class; });

nemesis::hkbBlendingTransitionEffectInternalState::hkbBlendingTransitionEffectInternalState() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkbBlendingTransitionEffectInternalState::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbBlendingTransitionEffectInternalState::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkReferencedObject::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8

    auto cur_ver = serializer.GetContentsVersion();

    if (cur_ver > HavokVersion::HK_2010_2_0)
    {
        serializer.Pad(16);
        serializer.WriteValue("fromPos", m_fromPos);
        serializer.WriteValue("fromRot", m_fromRot);
        serializer.WriteValue("toPos", m_toPos);
        serializer.WriteValue("toRot", m_toRot);
        serializer.WriteValue("lastPos", m_lastPos);
        serializer.WriteValue("lastRot", m_lastRot);
    }

    serializer.WriteObject("characterPoseAtBeginningOfTransition",
                           m_characterPoseAtBeginningOfTransition); // offset: 8/16 size: 12/16 align: 4/8
    serializer.WriteValue("timeRemaining", m_timeRemaining);        // offset: 20/32 size: 4/4 align: 4/4
    serializer.WriteValue("timeInTransition", m_timeInTransition);  // offset: 24/36 size: 4/4 align: 4/4

    if (cur_ver > HavokVersion::HK_2013_1_0)
    {
        if (cur_ver <= HavokVersion::HK_2013_2_0)
        {
            serializer.WriteValue("resetToGenerator", m_resetToGenerator);
        }

        serializer.WriteValue("toGeneratorSelfTranstitionMode", m_toGeneratorSelfTranstitionMode);
        serializer.WriteValue("initializeCharacterPose", m_initializeCharacterPose);
        serializer.WriteValue("alignThisFrame", m_alignThisFrame);
        serializer.WriteValue("alignmentFinished", m_alignmentFinished);
    }
    else
    {
        serializer.WriteValue("applySelfTransition",
                              m_applySelfTransition); // offset: 28/40 size: 1/1 align: 1/1
        serializer.WriteValue("initializeCharacterPose",
                              m_initializeCharacterPose); // offset: 29/41 size: 1/1 align: 1/1
    }

    serializer.Pad(
        cur_ver > HavokVersion::HK_2010_2_0 ? 16 : serializer.GetPointerSize()); // offset: 30/42 size: 2/6
    // class size: 32/48 align: 4/8
}

void nemesis::hkbBlendingTransitionEffectInternalState::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8

    auto cur_ver = deserializer.GetContentsVersion();

    if (cur_ver > HavokVersion::HK_2010_2_0)
    {
        deserializer.ReadValue("fromPos", m_fromPos);
        deserializer.ReadValue("fromRot", m_fromRot);
        deserializer.ReadValue("toPos", m_toPos);
        deserializer.ReadValue("toRot", m_toRot);
        deserializer.ReadValue("lastPos", m_lastPos);
        deserializer.ReadValue("lastRot", m_lastRot);
        deserializer.Pad(deserializer.GetPointerSize());
    }

    deserializer.ReadObject("characterPoseAtBeginningOfTransition",
                            m_characterPoseAtBeginningOfTransition); // offset: 8/16 size: 12/16 align: 4/8
    deserializer.ReadValue("timeRemaining", m_timeRemaining);        // offset: 20/32 size: 4/4 align: 4/4
    deserializer.ReadValue("timeInTransition", m_timeInTransition);  // offset: 24/36 size: 4/4 align: 4/4

    if (cur_ver > HavokVersion::HK_2013_1_0)
    {
        if (cur_ver <= HavokVersion::HK_2013_2_0)
        {
            deserializer.ReadValue("resetToGenerator", m_resetToGenerator);
        }

        deserializer.ReadValue("toGeneratorSelfTranstitionMode", m_toGeneratorSelfTranstitionMode);
        deserializer.ReadValue("initializeCharacterPose", m_initializeCharacterPose);
        deserializer.ReadValue("alignThisFrame", m_alignThisFrame);
        deserializer.ReadValue("alignmentFinished", m_alignmentFinished);
    }
    else
    {
        deserializer.ReadValue("applySelfTransition",
                               m_applySelfTransition); // offset: 28/40 size: 1/1 align: 1/1
        deserializer.ReadValue("initializeCharacterPose",
                               m_initializeCharacterPose); // offset: 29/41 size: 1/1 align: 1/1
    }

    deserializer.Pad(
        cur_ver > HavokVersion::HK_2010_2_0 ? 16 : deserializer.GetPointerSize()); // offset: 30/42 size: 2/6
    // class size: 32/48 align: 4/8
}
