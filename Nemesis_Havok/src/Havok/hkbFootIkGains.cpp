#include "Havok/hkbFootIkGains.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbFootIkGains,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbFootIkGains::Class; });

nemesis::hkbFootIkGains::hkbFootIkGains() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkbFootIkGains::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbFootIkGains::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("onOffGain", m_onOffGain);                       // offset: 0/0 size: 4/4 align: 4/4
    serializer.WriteValue("groundAscendingGain", m_groundAscendingGain);   // offset: 4/4 size: 4/4 align: 4/4
    serializer.WriteValue("groundDescendingGain", m_groundDescendingGain); // offset: 8/8 size: 4/4 align: 4/4
    serializer.WriteValue("footPlantedGain", m_footPlantedGain); // offset: 12/12 size: 4/4 align: 4/4
    serializer.WriteValue("footRaisedGain", m_footRaisedGain);   // offset: 16/16 size: 4/4 align: 4/4
    serializer.WriteValue(serializer.GetContentsVersion() > HavokVersion::HK_2013_3_0 ? "footLockingGain"
                                                                                      : "footUnlockGain",
                          m_footUnlockGain); // offset: 20/20 size: 4/4 align: 4/4
    serializer.WriteValue("worldFromModelFeedbackGain",
                          m_worldFromModelFeedbackGain);         // offset: 24/24 size: 4/4 align: 4/4
    serializer.WriteValue("errorUpDownBias", m_errorUpDownBias); // offset: 28/28 size: 4/4 align: 4/4
    serializer.WriteValue("alignWorldFromModelGain",
                          m_alignWorldFromModelGain);                  // offset: 32/32 size: 4/4 align: 4/4
    serializer.WriteValue("hipOrientationGain", m_hipOrientationGain); // offset: 36/36 size: 4/4 align: 4/4
    serializer.WriteValue("maxKneeAngleDifference",
                          m_maxKneeAngleDifference); // offset: 40/40 size: 4/4 align: 4/4
    serializer.WriteValue("ankleOrientationGain",
                          m_ankleOrientationGain); // offset: 44/44 size: 4/4 align: 4/4
    // class size: 48/48 align: 4/4
}

void nemesis::hkbFootIkGains::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("onOffGain", m_onOffGain);                     // offset: 0/0 size: 4/4 align: 4/4
    deserializer.ReadValue("groundAscendingGain", m_groundAscendingGain); // offset: 4/4 size: 4/4 align: 4/4
    deserializer.ReadValue("groundDescendingGain",
                           m_groundDescendingGain);               // offset: 8/8 size: 4/4 align: 4/4
    deserializer.ReadValue("footPlantedGain", m_footPlantedGain); // offset: 12/12 size: 4/4 align: 4/4
    deserializer.ReadValue("footRaisedGain", m_footRaisedGain);   // offset: 16/16 size: 4/4 align: 4/4
    deserializer.ReadValue(deserializer.GetContentsVersion() > HavokVersion::HK_2013_3_0 ? "footLockingGain"
                                                                                         : "footUnlockGain",
                           m_footUnlockGain); // offset: 20/20 size: 4/4 align: 4/4
    deserializer.ReadValue("worldFromModelFeedbackGain",
                           m_worldFromModelFeedbackGain);         // offset: 24/24 size: 4/4 align: 4/4
    deserializer.ReadValue("errorUpDownBias", m_errorUpDownBias); // offset: 28/28 size: 4/4 align: 4/4
    deserializer.ReadValue("alignWorldFromModelGain",
                           m_alignWorldFromModelGain);                  // offset: 32/32 size: 4/4 align: 4/4
    deserializer.ReadValue("hipOrientationGain", m_hipOrientationGain); // offset: 36/36 size: 4/4 align: 4/4
    deserializer.ReadValue("maxKneeAngleDifference",
                           m_maxKneeAngleDifference); // offset: 40/40 size: 4/4 align: 4/4
    deserializer.ReadValue("ankleOrientationGain",
                           m_ankleOrientationGain); // offset: 44/44 size: 4/4 align: 4/4
    // class size: 48/48 align: 4/4
}
