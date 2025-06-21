#include "Havok/hkxAnimatedVector.h"

REGISTER_HAVOK_POINTER_SOURCE(hkxAnimatedVector,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkxAnimatedVector::Class; });

nemesis::hkxAnimatedVector::hkxAnimatedVector() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkxAnimatedVector::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkxAnimatedVector::SerializeTo(nemesis::Serializer& serializer) const
{
    auto cur_ver = serializer.GetContentsVersion();

    if (cur_ver > HavokVersion::HK_660)
    {
        nemesis::hkReferencedObject::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8
    }

    if (cur_ver > HavokVersion::HK_2012_2_0)
    {
        if (m_vectors_1.IsEmpty())
        {
            for (size_t i = 0; i < m_vectors_0.GetSize(); ++i)
            {
                auto& vec4 = m_vectors_0[i];
                m_vectors_1.PushBack(new float(vec4.GetX()));
                m_vectors_1.PushBack(new float(vec4.GetY()));
                m_vectors_1.PushBack(new float(vec4.GetZ()));
                m_vectors_1.PushBack(new float(vec4.GetW()));
            }
        }

        serializer.WriteObject("vectors", m_vectors_1);
    }
    else
    {
        if (m_vectors_0.IsEmpty())
        {
            for (size_t i = 0; i < m_vectors_1.GetSize();)
            {
                auto& vec4 = m_vectors_0.PushBack(new nemesis::hkVector4());
                vec4.SetX(m_vectors_1[i++]);

                if (i >= m_vectors_1.GetSize()) break;

                vec4.SetY(m_vectors_1[i++]);

                if (i >= m_vectors_1.GetSize()) break;

                vec4.SetZ(m_vectors_1[i++]);

                if (i >= m_vectors_1.GetSize()) break;

                vec4.SetW(m_vectors_1[i++]);
            }
        }

        serializer.WriteObject("vectors", m_vectors_0); // offset: 8/16 size: 12/16 align: 4/8
    }

    serializer.WriteValue("hint", m_hint);                // offset: 20/32 size: 1/1 align: 1/1
    serializer.Pad(serializer.GetPointerSize());          // offset: 21/33 size: 3/7
    // class size: 24/40 align: 4/8
}

void nemesis::hkxAnimatedVector::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    auto cur_ver = deserializer.GetContentsVersion();

    if (cur_ver > HavokVersion::HK_660)
    {
        nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    }

    if (cur_ver > HavokVersion::HK_2012_2_0)
    {
        deserializer.ReadObject("vectors", m_vectors_1);
    }
    else
    {
        deserializer.ReadObject("vectors", m_vectors_0); // offset: 8/16 size: 12/16 align: 4/8
    }

    deserializer.ReadValue("hint", m_hint);                     // offset: 20/32 size: 1/1 align: 1/1
    deserializer.Pad(deserializer.GetPointerSize());            // offset: 21/33 size: 3/7
    // class size: 24/40 align: 4/8
}
