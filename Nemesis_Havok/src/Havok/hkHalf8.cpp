#include "Havok/hkHalf8.h"

REGISTER_HAVOK_POINTER_SOURCE(hkHalf8,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkHalf8::Class; });

nemesis::hkHalf8::hkHalf8() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkHalf8::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkHalf8::SerializeTo(nemesis::Serializer& serializer) const
{
    if (serializer.GetContentsVersion() > HavokVersion::HK_2010_2_0)
    {
        serializer.WriteSerializeIgnoredValue("halfs", m_halfs);
    }
    else if (serializer.GetContentsVersion() > HavokVersion::HK_2010_1_0)
    {
        serializer.WriteValue("quad", m_quad); // offset: 0/0 size: 16/16 align: 16/16
    }
    else
    {
        serializer.WriteValue("vec", m_quad);
    }

    // class size: 16/16 align: 16/16
}

void nemesis::hkHalf8::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    if (deserializer.GetContentsVersion() > HavokVersion::HK_2010_2_0)
    {
        deserializer.ReadValue("halfs", m_halfs);

        for (size_t i = 0; i < m_halfs.size(); ++i)
        {
            m_quad[i] = m_halfs[i];
        }
    }
    else if (deserializer.GetContentsVersion() > HavokVersion::HK_2010_1_0)
    {
        deserializer.ReadValue("quad", m_quad); // offset: 0/0 size: 16/16 align: 16/16

        for (size_t i = 0; i < m_halfs.size(); ++i)
        {
            m_halfs[i] = m_quad[i].AsShort(); 
        }
    }
    else
    {
        deserializer.ReadValue("vec", m_quad);

        for (size_t i = 0; i < m_halfs.size(); ++i)
        {
            m_halfs[i] = m_quad[i].AsShort();
        }
    }

    // class size: 16/16 align: 16/16
}
