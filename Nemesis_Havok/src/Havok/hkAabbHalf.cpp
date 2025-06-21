#include "Havok/hkAabbHalf.h"

REGISTER_HAVOK_POINTER_SOURCE(hkAabbHalf,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkAabbHalf::Class; });

nemesis::hkAabbHalf::hkAabbHalf() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkAabbHalf::GetClass(nemesis::HavokVersion version) const
{
    return version > HavokVersion::HK_2013_2_0 ? &Classes[1] : &Classes[0];
}

void nemesis::hkAabbHalf::SerializeTo(nemesis::Serializer& serializer) const
{
    if (serializer.GetContentsVersion() > HavokVersion::HK_2013_2_0)
    {
        std::array<unsigned short, 8> data8;

        for (size_t i = 0; i < m_data.size(); i++)
        {
            data8[i] = m_data[i];
        }

        for (size_t i = 0; i < m_extras.size(); i++)
        {
            data8[i + m_data.size()] = m_extras[i];
        }

        serializer.WriteValue("data", data8);
        return;
    }

    serializer.WriteValue("data", m_data);     // offset: 0/0 size: 12/12 align: 2/2
    serializer.WriteValue("extras", m_extras); // offset: 12/12 size: 4/4 align: 2/2
    // class size: 16/16 align: 2/2
}

void nemesis::hkAabbHalf::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    if (deserializer.GetContentsVersion() > HavokVersion::HK_2013_2_0)
    {
        std::array<unsigned short, 8> data8;
        deserializer.ReadValue("data", data8);

        for (size_t i = 0; i < m_data.size(); i++)
        {
            m_data[i] = data8[i];
        }

        for (size_t i = 0; i < m_extras.size(); i++)
        {
            m_extras[i] = data8[i + m_data.size()];
        }

        return;
    }

    deserializer.ReadValue("data", m_data);     // offset: 0/0 size: 12/12 align: 2/2
    deserializer.ReadValue("extras", m_extras); // offset: 12/12 size: 4/4 align: 2/2
    // class size: 16/16 align: 2/2
}
