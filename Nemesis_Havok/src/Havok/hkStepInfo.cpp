#include "Havok/hkStepInfo.h"

REGISTER_HAVOK_POINTER_SOURCE(hkStepInfo,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkStepInfo::Class; });

nemesis::hkStepInfo::hkStepInfo() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkStepInfo::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkStepInfo::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("startTime", m_startTime);       // offset: 0/0 size: 4/4 align: 4/4
    serializer.WriteValue("endTime", m_endTime);           // offset: 4/4 size: 4/4 align: 4/4
    serializer.WriteValue("deltaTime", m_deltaTime);       // offset: 8/8 size: 4/4 align: 4/4
    serializer.WriteValue("invDeltaTime", m_invDeltaTime); // offset: 12/12 size: 4/4 align: 4/4
    // class size: 16/16 align: 4/4
}

void nemesis::hkStepInfo::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("startTime", m_startTime);       // offset: 0/0 size: 4/4 align: 4/4
    deserializer.ReadValue("endTime", m_endTime);           // offset: 4/4 size: 4/4 align: 4/4
    deserializer.ReadValue("deltaTime", m_deltaTime);       // offset: 8/8 size: 4/4 align: 4/4
    deserializer.ReadValue("invDeltaTime", m_invDeltaTime); // offset: 12/12 size: 4/4 align: 4/4
    // class size: 16/16 align: 4/4
}
