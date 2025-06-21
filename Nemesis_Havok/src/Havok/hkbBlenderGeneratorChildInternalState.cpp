#include "Havok/hkbBlenderGeneratorChildInternalState.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbBlenderGeneratorChildInternalState,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbBlenderGeneratorChildInternalState::Class; });

nemesis::hkbBlenderGeneratorChildInternalState::hkbBlenderGeneratorChildInternalState() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkbBlenderGeneratorChildInternalState::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbBlenderGeneratorChildInternalState::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("isActive", m_isActive);           // offset: 0/0 size: 1/1 align: 1/1
    serializer.WriteValue("syncNextFrame", m_syncNextFrame); // offset: 1/1 size: 1/1 align: 1/1
    // class size: 2/2 align: 1/1
}

void nemesis::hkbBlenderGeneratorChildInternalState::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("isActive", m_isActive);           // offset: 0/0 size: 1/1 align: 1/1
    deserializer.ReadValue("syncNextFrame", m_syncNextFrame); // offset: 1/1 size: 1/1 align: 1/1
    // class size: 2/2 align: 1/1
}
