#include "Havok/hkbPoseMatchingGenerator.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbPoseMatchingGenerator,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbPoseMatchingGenerator::Class; });

nemesis::hkbPoseMatchingGenerator::hkbPoseMatchingGenerator() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkbBlenderGenerator(Class)
{
}

const nemesis::hkClass* nemesis::hkbPoseMatchingGenerator::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbPoseMatchingGenerator::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkbBlenderGenerator::SerializeTo(serializer); // offset: 0/0 size: 116/160 align: 4/8
    serializer.Pad(16);                                    // offset: 116/160 size: 12/0

    serializer.WriteValue("worldFromModelRotation",
                          m_worldFromModelRotation);               // offset: 128/160 size: 16/16 align: 16/16
    serializer.WriteValue("blendSpeed", m_blendSpeed);             // offset: 144/176 size: 4/4 align: 4/4
    serializer.WriteValue("minSpeedToSwitch", m_minSpeedToSwitch); // offset: 148/180 size: 4/4 align: 4/4
    serializer.WriteValue("minSwitchTimeNoError",
                          m_minSwitchTimeNoError); // offset: 152/184 size: 4/4 align: 4/4
    serializer.WriteValue("minSwitchTimeFullError",
                          m_minSwitchTimeFullError); // offset: 156/188 size: 4/4 align: 4/4
    serializer.WriteValue("startPlayingEventId",
                          m_startPlayingEventId); // offset: 160/192 size: 4/4 align: 4/4

    auto cur_ver = serializer.GetContentsVersion();

    if (cur_ver > HavokVersion::HK_2010_1_0)
    {
        serializer.WriteValue("startMatchingEventId",
                              m_startMatchingEventId); // offset: 164/196 size: 4/4 align: 4/4
    }

    serializer.WriteValue("rootBoneIndex", m_rootBoneIndex);       // offset: 168/200 size: 2/2 align: 2/2
    serializer.WriteValue("otherBoneIndex", m_otherBoneIndex);     // offset: 170/202 size: 2/2 align: 2/2
    serializer.WriteValue("anotherBoneIndex", m_anotherBoneIndex); // offset: 172/204 size: 2/2 align: 2/2
    serializer.WriteValue("pelvisIndex", m_pelvisIndex);           // offset: 174/206 size: 2/2 align: 2/2
    serializer.WriteValue("mode", m_mode);                         // offset: 176/208 size: 1/1 align: 1/1
    serializer.Pad(4);                                             // offset: 177/209 size: 3/3

    if (cur_ver > HavokVersion::HK_710)
    {
        serializer.WriteSerializeIgnoredValue("currentMatch",
                                              m_currentMatch); // offset: 180/212 size: 4/4 align: 4/4
        serializer.WriteSerializeIgnoredValue("bestMatch",
                                              m_bestMatch); // offset: 184/216 size: 4/4 align: 4/4
        serializer.WriteSerializeIgnoredValue("timeSinceBetterMatch",
                                              m_timeSinceBetterMatch); // offset: 188/220 size: 4/4 align: 4/4
        serializer.WriteSerializeIgnoredValue("error", m_error);       // offset: 192/224 size: 4/4 align: 4/4
        serializer.WriteSerializeIgnoredValue(
            "resetCurrentMatchLocalTime",
            m_resetCurrentMatchLocalTime); // offset: 196/228 size: 1/1 align: 1/1
        serializer.Pad(4);                 // offset: 197/229 size: 3/3
    }

    serializer.WriteSerializeIgnoredObject("poseMatchingUtility",
                                           m_poseMatchingUtility); // offset: 200/232 size: 4/8 align: 4/8
    serializer.Pad(16);                                            // offset: 204/240 size: 4/0
    // class size: 208/240 align: 16/16
}

void nemesis::hkbPoseMatchingGenerator::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkbBlenderGenerator::DeserializeFrom(deserializer); // offset: 0/0 size: 116/160 align: 4/8
    deserializer.Pad(16);                                        // offset: 116/160 size: 12/0

    deserializer.ReadValue("worldFromModelRotation",
                           m_worldFromModelRotation);   // offset: 128/160 size: 16/16 align: 16/16
    deserializer.ReadValue("blendSpeed", m_blendSpeed); // offset: 144/176 size: 4/4 align: 4/4
    deserializer.ReadValue("minSpeedToSwitch", m_minSpeedToSwitch); // offset: 148/180 size: 4/4 align: 4/4
    deserializer.ReadValue("minSwitchTimeNoError",
                           m_minSwitchTimeNoError); // offset: 152/184 size: 4/4 align: 4/4
    deserializer.ReadValue("minSwitchTimeFullError",
                           m_minSwitchTimeFullError); // offset: 156/188 size: 4/4 align: 4/4
    deserializer.ReadValue("startPlayingEventId",
                           m_startPlayingEventId); // offset: 160/192 size: 4/4 align: 4/4

    auto cur_ver = deserializer.GetContentsVersion();

    if (cur_ver > HavokVersion::HK_2010_1_0)
    {
        deserializer.ReadValue("startMatchingEventId",
                               m_startMatchingEventId); // offset: 164/196 size: 4/4 align: 4/4
    }

    deserializer.ReadValue("rootBoneIndex", m_rootBoneIndex);       // offset: 168/200 size: 2/2 align: 2/2
    deserializer.ReadValue("otherBoneIndex", m_otherBoneIndex);     // offset: 170/202 size: 2/2 align: 2/2
    deserializer.ReadValue("anotherBoneIndex", m_anotherBoneIndex); // offset: 172/204 size: 2/2 align: 2/2
    deserializer.ReadValue("pelvisIndex", m_pelvisIndex);           // offset: 174/206 size: 2/2 align: 2/2
    deserializer.ReadValue("mode", m_mode);                         // offset: 176/208 size: 1/1 align: 1/1
    deserializer.Pad(4);                                            // offset: 177/209 size: 3/3

    if (cur_ver > HavokVersion::HK_710)
    {
        deserializer.ReadValue("currentMatch", m_currentMatch); // offset: 180/212 size: 4/4 align: 4/4
        deserializer.ReadValue("bestMatch", m_bestMatch);       // offset: 184/216 size: 4/4 align: 4/4
        deserializer.ReadValue("timeSinceBetterMatch",
                               m_timeSinceBetterMatch); // offset: 188/220 size: 4/4 align: 4/4
        deserializer.ReadValue("error", m_error);       // offset: 192/224 size: 4/4 align: 4/4
        deserializer.ReadValue("resetCurrentMatchLocalTime",
                               m_resetCurrentMatchLocalTime); // offset: 196/228 size: 1/1 align: 1/1
        deserializer.Pad(4);                                  // offset: 197/229 size: 3/3
    }

    deserializer.ReadObject("poseMatchingUtility",
                            m_poseMatchingUtility); // offset: 200/232 size: 4/8 align: 4/8
    deserializer.Pad(16);                           // offset: 204/240 size: 4/0
    // class size: 208/240 align: 16/16
}
