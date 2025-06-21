#include "Havok/hkbFootIkModifierLeg.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbFootIkModifierLeg,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbFootIkModifierLeg::Class; });

nemesis::hkbFootIkModifierLeg::hkbFootIkModifierLeg() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkbFootIkModifierLeg::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbFootIkModifierLeg::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("originalAnkleTransformMS",
                          m_originalAnkleTransformMS); // offset: 0/0 size: 48/48 align: 16/16
    serializer.WriteSerializeIgnoredValue("prevAnkleRotLS",
                                          m_prevAnkleRotLS);      // offset: 48/48 size: 16/16 align: 16/16
    serializer.WriteValue("kneeAxisLS", m_kneeAxisLS);            // offset: 64/64 size: 16/16 align: 16/16
    serializer.WriteValue("footEndLS", m_footEndLS);              // offset: 80/80 size: 16/16 align: 16/16
    serializer.WriteObject("ungroundedEvent", m_ungroundedEvent); // offset: 96/96 size: 8/16 align: 4/8
    serializer.WriteValue("footPlantedAnkleHeightMS",
                          m_footPlantedAnkleHeightMS); // offset: 104/112 size: 4/4 align: 4/4
    serializer.WriteValue("footRaisedAnkleHeightMS",
                          m_footRaisedAnkleHeightMS);              // offset: 108/116 size: 4/4 align: 4/4
    serializer.WriteValue("maxAnkleHeightMS", m_maxAnkleHeightMS); // offset: 112/120 size: 4/4 align: 4/4
    serializer.WriteValue("minAnkleHeightMS", m_minAnkleHeightMS); // offset: 116/124 size: 4/4 align: 4/4
    serializer.WriteValue("maxKneeAngleDegrees",
                          m_maxKneeAngleDegrees); // offset: 120/128 size: 4/4 align: 4/4
    serializer.WriteValue("minKneeAngleDegrees",
                          m_minKneeAngleDegrees);            // offset: 124/132 size: 4/4 align: 4/4
    serializer.WriteValue("verticalError", m_verticalError); // offset: 128/136 size: 4/4 align: 4/4
    serializer.WriteValue("maxAnkleAngleDegrees",
                          m_maxAnkleAngleDegrees);         // offset: 132/140 size: 4/4 align: 4/4
    serializer.WriteValue("hipIndex", m_hipIndex);         // offset: 136/144 size: 2/2 align: 2/2
    serializer.WriteValue("kneeIndex", m_kneeIndex);       // offset: 138/146 size: 2/2 align: 2/2
    serializer.WriteValue("ankleIndex", m_ankleIndex);     // offset: 140/148 size: 2/2 align: 2/2
    serializer.WriteValue("hitSomething", m_hitSomething); // offset: 142/150 size: 1/1 align: 1/1
    serializer.WriteValue("isPlantedMS", m_isPlantedMS);   // offset: 143/151 size: 1/1 align: 1/1
    serializer.WriteValue("isOriginalAnkleTransformMSSet",
                          m_isOriginalAnkleTransformMSSet); // offset: 144/152 size: 1/1 align: 1/1
    serializer.Pad(16);                                     // offset: 145/153 size: 15/7
    // class size: 160/160 align: 16/16
}

void nemesis::hkbFootIkModifierLeg::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("originalAnkleTransformMS",
                           m_originalAnkleTransformMS);            // offset: 0/0 size: 48/48 align: 16/16
    deserializer.ReadValue("prevAnkleRotLS", m_prevAnkleRotLS);    // offset: 48/48 size: 16/16 align: 16/16
    deserializer.ReadValue("kneeAxisLS", m_kneeAxisLS);            // offset: 64/64 size: 16/16 align: 16/16
    deserializer.ReadValue("footEndLS", m_footEndLS);              // offset: 80/80 size: 16/16 align: 16/16
    deserializer.ReadObject("ungroundedEvent", m_ungroundedEvent); // offset: 96/96 size: 8/16 align: 4/8
    deserializer.ReadValue("footPlantedAnkleHeightMS",
                           m_footPlantedAnkleHeightMS); // offset: 104/112 size: 4/4 align: 4/4
    deserializer.ReadValue("footRaisedAnkleHeightMS",
                           m_footRaisedAnkleHeightMS);              // offset: 108/116 size: 4/4 align: 4/4
    deserializer.ReadValue("maxAnkleHeightMS", m_maxAnkleHeightMS); // offset: 112/120 size: 4/4 align: 4/4
    deserializer.ReadValue("minAnkleHeightMS", m_minAnkleHeightMS); // offset: 116/124 size: 4/4 align: 4/4
    deserializer.ReadValue("maxKneeAngleDegrees",
                           m_maxKneeAngleDegrees); // offset: 120/128 size: 4/4 align: 4/4
    deserializer.ReadValue("minKneeAngleDegrees",
                           m_minKneeAngleDegrees);            // offset: 124/132 size: 4/4 align: 4/4
    deserializer.ReadValue("verticalError", m_verticalError); // offset: 128/136 size: 4/4 align: 4/4
    deserializer.ReadValue("maxAnkleAngleDegrees",
                           m_maxAnkleAngleDegrees);         // offset: 132/140 size: 4/4 align: 4/4
    deserializer.ReadValue("hipIndex", m_hipIndex);         // offset: 136/144 size: 2/2 align: 2/2
    deserializer.ReadValue("kneeIndex", m_kneeIndex);       // offset: 138/146 size: 2/2 align: 2/2
    deserializer.ReadValue("ankleIndex", m_ankleIndex);     // offset: 140/148 size: 2/2 align: 2/2
    deserializer.ReadValue("hitSomething", m_hitSomething); // offset: 142/150 size: 1/1 align: 1/1
    deserializer.ReadValue("isPlantedMS", m_isPlantedMS);   // offset: 143/151 size: 1/1 align: 1/1
    deserializer.ReadValue("isOriginalAnkleTransformMSSet",
                           m_isOriginalAnkleTransformMSSet); // offset: 144/152 size: 1/1 align: 1/1
    deserializer.Pad(16);                                    // offset: 145/153 size: 15/7
    // class size: 160/160 align: 16/16
}
