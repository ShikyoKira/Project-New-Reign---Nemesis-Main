#include "Havok/hkbFootIkControlData.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbFootIkControlData,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbFootIkControlData::Class; });

nemesis::hkbFootIkControlData::hkbFootIkControlData() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkbFootIkControlData::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbFootIkControlData::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteObject("gains", m_gains); // offset: 0/0 size: 48/48 align: 16/16

    if (serializer.GetContentsVersion() > HavokVersion::HK_2013_3_0)
    {
        serializer.WriteValue("enabled", m_enabled);
    }

    // class size: 48/48 align: 16/16
}

void nemesis::hkbFootIkControlData::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadObject("gains", m_gains); // offset: 0/0 size: 48/48 align: 16/16

    if (deserializer.GetContentsVersion() > HavokVersion::HK_2013_3_0)
    {
        deserializer.ReadValue("enabled", m_enabled);
    }

    // class size: 48/48 align: 16/16
}
