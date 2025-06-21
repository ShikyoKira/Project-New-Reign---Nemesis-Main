#include "Havok/hkpModifierConstraintAtom.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpModifierConstraintAtom,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpModifierConstraintAtom::Class; });

nemesis::hkpModifierConstraintAtom::hkpModifierConstraintAtom() noexcept
    : nemesis::hkpModifierConstraintAtom(Class)
{
}

nemesis::hkpModifierConstraintAtom::hkpModifierConstraintAtom(const nemesis::hkClass& cls) noexcept
    : nemesis::HavokObject(cls.GetSignature())
    , nemesis::hkpConstraintAtom(cls)
{
}

const nemesis::hkClass* nemesis::hkpModifierConstraintAtom::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpModifierConstraintAtom::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpConstraintAtom::SerializeTo(serializer); // offset: 0/0 size: 2/2 align: 2/2
    serializer.Skip(14);                                 // offset: 2/2 size: 14/14

    serializer.WriteValue("modifierAtomSize", m_modifierAtomSize); // offset: 16/16 size: 2/2 align: 16/16
    serializer.WriteValue("childSize", m_childSize);               // offset: 18/18 size: 2/2 align: 2/2
    serializer.Pad(serializer.GetPointerSize());                   // offset: 20/20 size: 0/4

    serializer.WriteObject("child", m_child); // offset: 20/24 size: 4/8 align: 4/8

    if (serializer.GetContentsVersion() <= HavokVersion::HK_2011_3_0)
    {
        serializer.WriteValue("pad", m_pad); // offset: 24/32 size: 8/8 align: 4/4
    }

    serializer.Pad(16); // offset: 32/40 size: 0/8
    // class size: 32/48 align: 16/16
}

void nemesis::hkpModifierConstraintAtom::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpConstraintAtom::DeserializeFrom(deserializer); // offset: 0/0 size: 2/2 align: 2/2
    deserializer.Skip(14);                                     // offset: 2/2 size: 14/14

    deserializer.ReadValue("modifierAtomSize", m_modifierAtomSize); // offset: 16/16 size: 2/2 align: 16/16
    deserializer.ReadValue("childSize", m_childSize);               // offset: 18/18 size: 2/2 align: 2/2
    deserializer.Pad(deserializer.GetPointerSize());                // offset: 20/20 size: 0/4

    deserializer.ReadObject("child", m_child); // offset: 20/24 size: 4/8 align: 4/8

    if (deserializer.GetContentsVersion() <= HavokVersion::HK_2011_3_0)
    {
        deserializer.ReadValue("pad", m_pad); // offset: 24/32 size: 8/8 align: 4/4
    }

    deserializer.Pad(16); // offset: 32/40 size: 0/8
    // class size: 32/48 align: 16/16
}
