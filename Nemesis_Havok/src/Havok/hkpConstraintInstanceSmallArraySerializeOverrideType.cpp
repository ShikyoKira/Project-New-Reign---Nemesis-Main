#include "Havok/hkpConstraintInstanceSmallArraySerializeOverrideType.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpConstraintInstanceSmallArraySerializeOverrideType,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpConstraintInstanceSmallArraySerializeOverrideType::Class; });

nemesis::hkpConstraintInstanceSmallArraySerializeOverrideType::hkpConstraintInstanceSmallArraySerializeOverrideType() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkpConstraintInstanceSmallArraySerializeOverrideType::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpConstraintInstanceSmallArraySerializeOverrideType::SerializeTo(
    nemesis::Serializer& serializer) const
{
    if (serializer.GetContentsVersion() > HavokVersion::HK_660)
    {
        serializer.WriteSerializeIgnoredObject("data", m_data); // offset: 0/0 size: 4/8 align: 4/8
    }
    else
    {
        serializer.WriteObject("data", m_data);
    }

    serializer.WriteValue("size", m_size);                         // offset: 4/8 size: 2/2 align: 2/2
    serializer.WriteValue("capacityAndFlags", m_capacityAndFlags); // offset: 6/10 size: 2/2 align: 2/2
    serializer.Pad(serializer.GetPointerSize());                   // offset: 8/12 size: 0/4
    // class size: 8/16 align: 4/8
}

void nemesis::hkpConstraintInstanceSmallArraySerializeOverrideType::DeserializeFrom(
    nemesis::Deserializer& deserializer)
{
    deserializer.ReadObject("data", m_data);                        // offset: 0/0 size: 4/8 align: 4/8
    deserializer.ReadValue("size", m_size);                         // offset: 4/8 size: 2/2 align: 2/2
    deserializer.ReadValue("capacityAndFlags", m_capacityAndFlags); // offset: 6/10 size: 2/2 align: 2/2
    deserializer.Pad(deserializer.GetPointerSize());                // offset: 8/12 size: 0/4
    // class size: 8/16 align: 4/8
}
