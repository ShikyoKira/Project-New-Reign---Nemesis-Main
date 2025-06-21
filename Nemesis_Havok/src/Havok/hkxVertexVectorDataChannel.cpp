#include "Havok/hkxVertexVectorDataChannel.h"

REGISTER_HAVOK_POINTER_SOURCE(hkxVertexVectorDataChannel,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkxVertexVectorDataChannel::Class; });

nemesis::hkxVertexVectorDataChannel::hkxVertexVectorDataChannel() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkxVertexVectorDataChannel::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkxVertexVectorDataChannel::SerializeTo(nemesis::Serializer& serializer) const
{
    auto cur_ver = serializer.GetContentsVersion();

    if (cur_ver > HavokVersion::HK_660)
    {
        nemesis::hkReferencedObject::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8
    }

    if (cur_ver > HavokVersion::HK_2012_2_0)
    {
        if (m_perVertexVectors_1.IsEmpty())
        {
            for (auto& vec4 : m_perVertexVectors_0)
            {
                m_perVertexVectors_1.PushBack(new float(vec4->GetX()));
                m_perVertexVectors_1.PushBack(new float(vec4->GetY()));
                m_perVertexVectors_1.PushBack(new float(vec4->GetZ()));
                m_perVertexVectors_1.PushBack(new float(vec4->GetW()));
            }
        }

        serializer.WriteObject("perVertexVectors", m_perVertexVectors_1);
    }
    else
    {
        if (m_perVertexVectors_0.IsEmpty())
        {
            for (size_t i = 0; i < m_perVertexVectors_1.GetSize();)
            {
                auto& vec4 = m_perVertexVectors_0.PushBack(new nemesis::hkVector4());
                vec4.SetX(m_perVertexVectors_1[i++]);

                if (i >= m_perVertexVectors_1.GetSize()) break;

                vec4.SetY(m_perVertexVectors_1[i++]);

                if (i >= m_perVertexVectors_1.GetSize()) break;

                vec4.SetZ(m_perVertexVectors_1[i++]);

                if (i >= m_perVertexVectors_1.GetSize()) break;

                vec4.SetW(m_perVertexVectors_1[i++]);
            }
        }

        serializer.WriteObject("perVertexVectors",
                               m_perVertexVectors_0); // offset: 8/16 size: 12/16 align: 4/8
    }

    // class size: 20/32 align: 4/8
}

void nemesis::hkxVertexVectorDataChannel::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    auto cur_ver = deserializer.GetContentsVersion();

    if (cur_ver > HavokVersion::HK_660)
    {
        nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    }

    if (cur_ver > HavokVersion::HK_2012_2_0)
    {
        deserializer.ReadObject("perVertexVectors", m_perVertexVectors_1);
    }
    else
    {
        deserializer.ReadObject("perVertexVectors",
                                m_perVertexVectors_0); // offset: 8/16 size: 12/16 align: 4/8
    }

    // class size: 20/32 align: 4/8
}
