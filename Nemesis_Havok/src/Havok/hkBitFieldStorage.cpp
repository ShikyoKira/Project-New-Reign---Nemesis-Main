#include "Havok/hkBitFieldStorage.h"

#include "Havok/hkBitField.h"

nemesis::hkBitFieldStorage::hkBitFieldStorage(nemesis::hkBitField& parent) noexcept
    : nemesis::HavokObject(parent.Class.GetSignature())
    , m_words(parent.m_words)
    , m_numBits(parent.m_numBits)
{
}

const nemesis::hkClass* nemesis::hkBitFieldStorage::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkBitFieldStorage::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteObject("words", m_words);    // offset: 0/0 size: 12/16 align: 4/8
    serializer.WriteValue("numBits", m_numBits); // offset: 12/16 size: 4/4 align: 4/4
    serializer.Pad(serializer.GetPointerSize()); // offset: 16/20 size: 0/4
    // class size: 16/24 align: 4/8
}

void nemesis::hkBitFieldStorage::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadObject("words", m_words);       // offset: 0/0 size: 12/16 align: 4/8
    deserializer.ReadValue("numBits", m_numBits);    // offset: 12/16 size: 4/4 align: 4/4
    deserializer.Pad(deserializer.GetPointerSize()); // offset: 16/20 size: 0/4
    // class size: 16/24 align: 4/8
}
