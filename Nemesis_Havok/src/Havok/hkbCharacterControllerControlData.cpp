#include "Havok/hkbCharacterControllerControlData.h"
#include "Havok/hkbCharacterControllerModifierControlData.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbCharacterControllerControlData,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbCharacterControllerControlData::Class; });

nemesis::hkbCharacterControllerControlData::hkbCharacterControllerControlData() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkbCharacterControllerControlData::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbCharacterControllerControlData::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("desiredVelocity", m_desiredVelocity); // offset: 0/0 size: 16/16 align: 16/16
    serializer.WriteValue("verticalGain", m_verticalGain);       // offset: 16/16 size: 4/4 align: 4/4
    serializer.WriteValue("horizontalCatchUpGain",
                          m_horizontalCatchUpGain); // offset: 20/20 size: 4/4 align: 4/4
    serializer.WriteValue("maxVerticalSeparation",
                          m_maxVerticalSeparation); // offset: 24/24 size: 4/4 align: 4/4
    serializer.WriteValue("maxHorizontalSeparation",
                          m_maxHorizontalSeparation); // offset: 28/28 size: 4/4 align: 4/4
    // class size: 32/32 align: 16/16
}

void nemesis::hkbCharacterControllerControlData::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("desiredVelocity", m_desiredVelocity); // offset: 0/0 size: 16/16 align: 16/16
    deserializer.ReadValue("verticalGain", m_verticalGain);       // offset: 16/16 size: 4/4 align: 4/4
    deserializer.ReadValue("horizontalCatchUpGain",
                           m_horizontalCatchUpGain); // offset: 20/20 size: 4/4 align: 4/4
    deserializer.ReadValue("maxVerticalSeparation",
                           m_maxVerticalSeparation); // offset: 24/24 size: 4/4 align: 4/4
    deserializer.ReadValue("maxHorizontalSeparation",
                           m_maxHorizontalSeparation); // offset: 28/28 size: 4/4 align: 4/4
    // class size: 32/32 align: 16/16
}

void nemesis::hkbCharacterControllerControlData::CopyFrom(
    const nemesis::hkbCharacterControllerModifierControlData& control_data)
{
    m_verticalGain            = control_data.m_verticalGain;
    m_horizontalCatchUpGain   = control_data.m_horizontalCatchUpGain;
    m_maxVerticalSeparation   = control_data.m_maxVerticalSeparation;
    m_maxHorizontalSeparation = control_data.m_maxHorizontalSeparation;
}
