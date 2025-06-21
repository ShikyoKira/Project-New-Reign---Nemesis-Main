#include "Havok/hkbCharacterControllerModifierControlData.h"
#include "Havok/hkbCharacterControllerControlData.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbCharacterControllerModifierControlData,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbCharacterControllerModifierControlData::Class; });

nemesis::hkbCharacterControllerModifierControlData::hkbCharacterControllerModifierControlData() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkbCharacterControllerModifierControlData::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbCharacterControllerModifierControlData::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("verticalGain", m_verticalGain); // offset: 0/0 size: 4/4 align: 4/4
    serializer.WriteValue("horizontalCatchUpGain",
                          m_horizontalCatchUpGain); // offset: 4/4 size: 4/4 align: 4/4
    serializer.WriteValue("maxVerticalSeparation",
                          m_maxVerticalSeparation); // offset: 8/8 size: 4/4 align: 4/4
    serializer.WriteValue("maxHorizontalSeparation",
                          m_maxHorizontalSeparation); // offset: 12/12 size: 4/4 align: 4/4
    // class size: 16/16 align: 4/4
}

void nemesis::hkbCharacterControllerModifierControlData::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("verticalGain", m_verticalGain); // offset: 0/0 size: 4/4 align: 4/4
    deserializer.ReadValue("horizontalCatchUpGain",
                           m_horizontalCatchUpGain); // offset: 4/4 size: 4/4 align: 4/4
    deserializer.ReadValue("maxVerticalSeparation",
                           m_maxVerticalSeparation); // offset: 8/8 size: 4/4 align: 4/4
    deserializer.ReadValue("maxHorizontalSeparation",
                           m_maxHorizontalSeparation); // offset: 12/12 size: 4/4 align: 4/4
    // class size: 16/16 align: 4/4
}

void nemesis::hkbCharacterControllerModifierControlData::CopyFrom(
    const nemesis::hkbCharacterControllerControlData& control_data)
{
    m_verticalGain            = control_data.m_verticalGain;
    m_horizontalCatchUpGain   = control_data.m_horizontalCatchUpGain;
    m_maxVerticalSeparation   = control_data.m_maxVerticalSeparation;
    m_maxHorizontalSeparation = control_data.m_maxHorizontalSeparation;
}
