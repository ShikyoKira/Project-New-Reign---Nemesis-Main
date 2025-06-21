#include "Havok/hkbFootIkModifierInternalLegData.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbFootIkModifierInternalLegData,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbFootIkModifierInternalLegData::Class; });

nemesis::hkbFootIkModifierInternalLegData::hkbFootIkModifierInternalLegData() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkbFootIkModifierInternalLegData::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbFootIkModifierInternalLegData::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("groundPosition", m_groundPosition); // offset: 0/0 size: 16/16 align: 16/16
    serializer.WriteSerializeIgnoredObject("footIkSolver",
                                           m_footIkSolver); // offset: 16/16 size: 4/8 align: 4/8
    serializer.Pad(16);                                     // offset: 20/24 size: 12/8
    // class size: 32/32 align: 16/16
}

void nemesis::hkbFootIkModifierInternalLegData::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("groundPosition", m_groundPosition); // offset: 0/0 size: 16/16 align: 16/16
    deserializer.ReadObject("footIkSolver", m_footIkSolver);    // offset: 16/16 size: 4/8 align: 4/8
    deserializer.Pad(16);                                       // offset: 20/24 size: 12/8
    // class size: 32/32 align: 16/16
}
