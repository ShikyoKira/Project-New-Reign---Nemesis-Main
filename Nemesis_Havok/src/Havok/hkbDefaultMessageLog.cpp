#include "Havok/hkbDefaultMessageLog.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbDefaultMessageLog,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbDefaultMessageLog::Class; });

nemesis::hkbDefaultMessageLog::hkbDefaultMessageLog() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkbDefaultMessageLog::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbDefaultMessageLog::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("", unk0); // offset: 0/0 size: 1/1 align: 1/1
    // class size: 1/1 align: 1/1
}

void nemesis::hkbDefaultMessageLog::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("", unk0); // offset: 0/0 size: 1/1 align: 1/1
    // class size: 1/1 align: 1/1
}
