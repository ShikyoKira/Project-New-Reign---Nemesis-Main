#include "Havok/hkpStiffSpringConstraintAtom.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpStiffSpringConstraintAtom,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpStiffSpringConstraintAtom::Class; });

nemesis::hkpStiffSpringConstraintAtom::hkpStiffSpringConstraintAtom() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkpConstraintAtom(Class)
{
}

const nemesis::hkClass* nemesis::hkpStiffSpringConstraintAtom::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpStiffSpringConstraintAtom::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpConstraintAtom::SerializeTo(serializer); // offset: 0/0 size: 2/2 align: 2/2
    serializer.Skip(2);                                  // offset: 2/2 size: 2/2

    serializer.WriteValue("length", m_length); // offset: 4/4 size: 4/4 align: 4/4

    if (serializer.GetContentsVersion() > HavokVersion::HK_2011_3_0)
    {
        serializer.WriteValue("maxLength", m_maxLength);
    }

    // class size: 8/8 align: 4/4
}

void nemesis::hkpStiffSpringConstraintAtom::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpConstraintAtom::DeserializeFrom(deserializer); // offset: 0/0 size: 2/2 align: 2/2
    deserializer.Skip(2);                                      // offset: 2/2 size: 2/2

    deserializer.ReadValue("length", m_length); // offset: 4/4 size: 4/4 align: 4/4

    if (deserializer.GetContentsVersion() > HavokVersion::HK_2011_3_0)
    {
        deserializer.ReadValue("maxLength", m_maxLength);
    }

    // class size: 8/8 align: 4/4
}
