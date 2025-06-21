#include "Havok/hkpBroadPhaseHandle.h"

nemesis::hkpBroadPhaseHandle::hkpBroadPhaseHandle(const nemesis::hkClass& cls) noexcept
    : nemesis::HavokObject(cls.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkpBroadPhaseHandle::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpBroadPhaseHandle::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteSerializeIgnoredValue("id", m_id); // offset: 0/0 size: 4/4 align: 4/4
    // class size: 4/4 align: 4/4
}

void nemesis::hkpBroadPhaseHandle::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("id", m_id); // offset: 0/0 size: 4/4 align: 4/4
    // class size: 4/4 align: 4/4
}
