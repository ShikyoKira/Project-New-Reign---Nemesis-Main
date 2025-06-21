#include "Havok/hkxSparselyAnimatedString.h"

REGISTER_HAVOK_POINTER_SOURCE(hkxSparselyAnimatedString,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkxSparselyAnimatedString::Class; });

nemesis::hkxSparselyAnimatedString::hkxSparselyAnimatedString() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkxSparselyAnimatedString::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkxSparselyAnimatedString::SerializeTo(nemesis::Serializer& serializer) const
{
    if (serializer.GetContentsVersion() > HavokVersion::HK_660)
    {
        nemesis::hkReferencedObject::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8

        if (m_strings_1.IsEmpty())
        {
            for (size_t i = 0; i < m_strings_0.GetSize(); i++)
            {
                m_strings_1.PushBack(new nemesis::hkCString(m_strings_0[i].m_string.GetValue()));
            }
        }

        serializer.WriteObject("strings", m_strings_1);       // offset: 8/16 size: 12/16 align: 4/8
    }
    else
    {
        if (m_strings_0.IsEmpty())
        {
            for (size_t i = 0; i < m_strings_1.GetSize(); i++)
            {
                auto& each = m_strings_0.PushBack(new nemesis::hkxSparselyAnimatedStringStringType());
                each.m_string.SetValue(m_strings_1[i].GetValue());
            }
        }

        serializer.WriteObject("strings", m_strings_0); // offset: 8/16 size: 12/16 align: 4/8
    }

    serializer.WriteObject("times", m_times);        // offset: 20/32 size: 12/16 align: 4/8
    // class size: 32/48 align: 4/8
}

void nemesis::hkxSparselyAnimatedString::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    if (deserializer.GetContentsVersion() > HavokVersion::HK_660)
    {
        nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
        deserializer.ReadObject("strings", m_strings_1);            // offset: 8/16 size: 12/16 align: 4/8
        m_strings_0.Clear();
    }
    else
    {
        deserializer.ReadObject("strings", m_strings_0);
        m_strings_1.Clear();
    }

    deserializer.ReadObject("times", m_times);        // offset: 20/32 size: 12/16 align: 4/8
    // class size: 32/48 align: 4/8
}
