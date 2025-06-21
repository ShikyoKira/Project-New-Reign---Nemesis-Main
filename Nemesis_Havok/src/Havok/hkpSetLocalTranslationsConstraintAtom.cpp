#include "Havok/hkpSetLocalTranslationsConstraintAtom.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpSetLocalTranslationsConstraintAtom,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpSetLocalTranslationsConstraintAtom::Class; });

nemesis::hkpSetLocalTranslationsConstraintAtom::hkpSetLocalTranslationsConstraintAtom() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkpConstraintAtom(Class)
{
}

const nemesis::hkClass* nemesis::hkpSetLocalTranslationsConstraintAtom::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpSetLocalTranslationsConstraintAtom::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpConstraintAtom::SerializeTo(serializer); // offset: 0/0 size: 2/2 align: 2/2
    serializer.Skip(14);                                 // offset: 2/2 size: 14/14

    serializer.WriteValue("translationA", m_translationA); // offset: 16/16 size: 16/16 align: 16/16
    serializer.WriteValue("translationB", m_translationB); // offset: 32/32 size: 16/16 align: 16/16
    // class size: 48/48 align: 16/16
}

void nemesis::hkpSetLocalTranslationsConstraintAtom::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpConstraintAtom::DeserializeFrom(deserializer); // offset: 0/0 size: 2/2 align: 2/2
    deserializer.Skip(14);                                     // offset: 2/2 size: 14/14

    deserializer.ReadValue("translationA", m_translationA); // offset: 16/16 size: 16/16 align: 16/16
    deserializer.ReadValue("translationB", m_translationB); // offset: 32/32 size: 16/16 align: 16/16
    // class size: 48/48 align: 16/16
}
