#include "Havok/hkSimpleProperty.h"

nemesis::hkPropertyCommon::hkPropertyCommon() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

void nemesis::hkPropertyCommon::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("key", m_key); // offset: 0/0 size: 4/4 align: 4/4

    if (serializer.GetContentsVersion() <= HavokVersion::HK_2010_2_0)
    {
        serializer.WriteValue("alignmentPadding", m_alignmentPadding); // offset: 4/4 size: 4/4 align: 4/4
    }

    // class size: 8/8 align: 4/4
}

void nemesis::hkPropertyCommon::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("key", m_key); // offset: 0/0 size: 4/4 align: 4/4

    if (deserializer.GetContentsVersion() <= HavokVersion::HK_2010_2_0)
    {
        deserializer.ReadValue("alignmentPadding", m_alignmentPadding); // offset: 4/4 size: 4/4 align: 4/4
    }

    // class size: 8/8 align: 4/4
}

REGISTER_HAVOK_POINTER_SOURCE(hkpProperty,
                              [](nemesis::HavokVersion version) { return &nemesis::hkpProperty::Class; });

const nemesis::hkClass* nemesis::hkpProperty::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpProperty::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkPropertyCommon::SerializeTo(serializer); // offset: 0/0 size: 8/8 align: 4/4
    serializer.WriteObject("value", m_value);           // offset: 8/8 size: 8/8 align: 8/8
    // class size: 16/16 align: 8/8
}

void nemesis::hkpProperty::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkPropertyCommon::DeserializeFrom(deserializer); // offset: 0/0 size: 8/8 align: 4/4
    deserializer.ReadObject("value", m_value);                // offset: 8/8 size: 8/8 align: 8/8
    // class size: 16/16 align: 8/8
}

REGISTER_HAVOK_POINTER_SOURCE(hkSimpleProperty,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkSimpleProperty::Class; });

const nemesis::hkClass* nemesis::hkSimpleProperty::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkSimpleProperty::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkPropertyCommon::SerializeTo(serializer); // offset: 0/0 size: 8/8 align: 4/4
    serializer.WriteObject("value", m_value);           // offset: 8/8 size: 8/8 align: 8/8
    // class size: 16/16 align: 8/8
}

void nemesis::hkSimpleProperty::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkPropertyCommon::DeserializeFrom(deserializer); // offset: 0/0 size: 8/8 align: 4/4
    deserializer.ReadObject("value", m_value);                // offset: 8/8 size: 8/8 align: 8/8
    // class size: 16/16 align: 8/8
}
