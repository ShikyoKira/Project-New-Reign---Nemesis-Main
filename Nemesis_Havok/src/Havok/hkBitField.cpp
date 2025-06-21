#include "Havok/hkBitField.h"

REGISTER_HAVOK_POINTER_SOURCE(hkBitField,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkBitField::Class; });

nemesis::hkBitField::hkBitField() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , m_storage(*this)
{
}

const nemesis::hkClass* nemesis::hkBitField::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkBitField::SerializeTo(nemesis::Serializer& serializer) const
{
    if (serializer.GetContentsVersion() > HavokVersion::HK_2013_1_0)
    {
        serializer.WriteObject("storage", m_storage);
        return;
    }

    serializer.WriteObject("words", m_words);    // offset: 0/0 size: 12/16 align: 4/8
    serializer.WriteValue("numBits", m_numBits); // offset: 12/16 size: 4/4 align: 4/4
    serializer.Pad(serializer.GetPointerSize()); // offset: 16/20 size: 0/4
    // class size: 16/24 align: 4/8
}

void nemesis::hkBitField::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    if (deserializer.GetContentsVersion() > HavokVersion::HK_2013_1_0)
    {
        deserializer.ReadObject("storage", m_storage);
        return;
    }

    deserializer.ReadObject("words", m_words);       // offset: 0/0 size: 12/16 align: 4/8
    deserializer.ReadValue("numBits", m_numBits);    // offset: 12/16 size: 4/4 align: 4/4
    deserializer.Pad(deserializer.GetPointerSize()); // offset: 16/20 size: 0/4
    // class size: 16/24 align: 4/8
}
