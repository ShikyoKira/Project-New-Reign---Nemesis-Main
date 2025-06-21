#include "Havok/Base/hkpConstraintAtom.h"

nemesis::hkpConstraintAtom::hkpConstraintAtom(const nemesis::hkClass& cls) noexcept
    : nemesis::HavokObject(cls.GetSignature())
{
}

void nemesis::hkpConstraintAtom::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("type", m_type); // offset: 0/0 size: 2/2 align: 2/2
    // class size: 2/2 align: 2/2
}

void nemesis::hkpConstraintAtom::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("type", m_type); // offset: 0/0 size: 2/2 align: 2/2
    // class size: 2/2 align: 2/2
}
