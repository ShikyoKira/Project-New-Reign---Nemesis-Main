#include "Havok/hkbFootIkDriverInfoLeg.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbFootIkDriverInfoLeg,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbFootIkDriverInfoLeg::Class; });

nemesis::hkbFootIkDriverInfoLeg::hkbFootIkDriverInfoLeg() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkbFootIkDriverInfoLeg::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbFootIkDriverInfoLeg::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteSerializeIgnoredValue("prevAnkleRotLS",
                                          m_prevAnkleRotLS); // offset: 0/0 size: 16/16 align: 16/16
    serializer.WriteValue("kneeAxisLS", m_kneeAxisLS);       // offset: 16/16 size: 16/16 align: 16/16
    serializer.WriteValue("footEndLS", m_footEndLS);         // offset: 32/32 size: 16/16 align: 16/16
    serializer.WriteValue("footPlantedAnkleHeightMS",
                          m_footPlantedAnkleHeightMS); // offset: 48/48 size: 4/4 align: 4/4
    serializer.WriteValue("footRaisedAnkleHeightMS",
                          m_footRaisedAnkleHeightMS);                    // offset: 52/52 size: 4/4 align: 4/4
    serializer.WriteValue("maxAnkleHeightMS", m_maxAnkleHeightMS);       // offset: 56/56 size: 4/4 align: 4/4
    serializer.WriteValue("minAnkleHeightMS", m_minAnkleHeightMS);       // offset: 60/60 size: 4/4 align: 4/4
    serializer.WriteValue("maxKneeAngleDegrees", m_maxKneeAngleDegrees); // offset: 64/64 size: 4/4 align: 4/4
    serializer.WriteValue("minKneeAngleDegrees", m_minKneeAngleDegrees); // offset: 68/68 size: 4/4 align: 4/4
    serializer.WriteValue("maxAnkleAngleDegrees",
                          m_maxAnkleAngleDegrees); // offset: 72/72 size: 4/4 align: 4/4

    if (serializer.GetContentsVersion() > HavokVersion::HK_2013_2_0)
    {
        serializer.WriteValue("hipIndex", m_hipIndex);
        serializer.WriteValue("hipSiblingIndex", m_hipSiblingIndex);
        serializer.WriteValue("kneeIndex", m_kneeIndex);
        serializer.WriteValue("kneeSiblingIndex", m_kneeSiblingIndex);
    }
    else
    {
        serializer.WriteValue("hipIndex", m_hipIndex);   // offset: 76/76 size: 2/2 align: 2/2
        serializer.WriteValue("kneeIndex", m_kneeIndex); // offset: 78/78 size: 2/2 align: 2/2
    }

    serializer.WriteValue("ankleIndex", m_ankleIndex); // offset: 80/80 size: 2/2 align: 2/2
    serializer.Pad(16);                                // offset: 82/82 size: 14/14
    // class size: 96/96 align: 16/16
}

void nemesis::hkbFootIkDriverInfoLeg::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("prevAnkleRotLS", m_prevAnkleRotLS); // offset: 0/0 size: 16/16 align: 16/16
    deserializer.ReadValue("kneeAxisLS", m_kneeAxisLS);         // offset: 16/16 size: 16/16 align: 16/16
    deserializer.ReadValue("footEndLS", m_footEndLS);           // offset: 32/32 size: 16/16 align: 16/16
    deserializer.ReadValue("footPlantedAnkleHeightMS",
                           m_footPlantedAnkleHeightMS); // offset: 48/48 size: 4/4 align: 4/4
    deserializer.ReadValue("footRaisedAnkleHeightMS",
                           m_footRaisedAnkleHeightMS);              // offset: 52/52 size: 4/4 align: 4/4
    deserializer.ReadValue("maxAnkleHeightMS", m_maxAnkleHeightMS); // offset: 56/56 size: 4/4 align: 4/4
    deserializer.ReadValue("minAnkleHeightMS", m_minAnkleHeightMS); // offset: 60/60 size: 4/4 align: 4/4
    deserializer.ReadValue("maxKneeAngleDegrees",
                           m_maxKneeAngleDegrees); // offset: 64/64 size: 4/4 align: 4/4
    deserializer.ReadValue("minKneeAngleDegrees",
                           m_minKneeAngleDegrees); // offset: 68/68 size: 4/4 align: 4/4
    deserializer.ReadValue("maxAnkleAngleDegrees",
                           m_maxAnkleAngleDegrees); // offset: 72/72 size: 4/4 align: 4/4

    if (deserializer.GetContentsVersion() > HavokVersion::HK_2013_2_0)
    {
        deserializer.ReadValue("hipIndex", m_hipIndex);
        deserializer.ReadValue("hipSiblingIndex", m_hipSiblingIndex);
        deserializer.ReadValue("kneeIndex", m_kneeIndex);
        deserializer.ReadValue("kneeSiblingIndex", m_kneeSiblingIndex);
    }
    else
    {
        deserializer.ReadValue("hipIndex", m_hipIndex);   // offset: 76/76 size: 2/2 align: 2/2
        deserializer.ReadValue("kneeIndex", m_kneeIndex); // offset: 78/78 size: 2/2 align: 2/2
    }

    deserializer.ReadValue("ankleIndex", m_ankleIndex); // offset: 80/80 size: 2/2 align: 2/2
    deserializer.Pad(16);                               // offset: 82/82 size: 14/14
    // class size: 96/96 align: 16/16
}
