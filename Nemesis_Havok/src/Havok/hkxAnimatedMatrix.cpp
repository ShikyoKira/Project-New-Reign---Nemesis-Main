#include "Havok/hkxAnimatedMatrix.h"

REGISTER_HAVOK_POINTER_SOURCE(hkxAnimatedMatrix,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkxAnimatedMatrix::Class; });

void nemesis::hkxAnimatedMatrix::Combine(nemesis::hkArray<float>& matrices, const nemesis::hkVector4& vec4)
{
    matrices.PushBack(new float(vec4.GetX()));
    matrices.PushBack(new float(vec4.GetY()));
    matrices.PushBack(new float(vec4.GetZ()));
    matrices.PushBack(new float(vec4.GetW()));
}

size_t nemesis::hkxAnimatedMatrix::Combine(nemesis::hkVector4& vec4,
                                           const nemesis::hkArray<float>& matrices,
                                           size_t index)
{
    vec4.SetX(matrices[index++]);

    if (index >= matrices.GetSize()) return index;

    vec4.SetY(matrices[index++]);

    if (index >= matrices.GetSize()) return index;

    vec4.SetZ(matrices[index++]);

    if (index >= matrices.GetSize()) return index;

    vec4.SetW(matrices[index++]);
    return index;
}

nemesis::hkxAnimatedMatrix::hkxAnimatedMatrix() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkxAnimatedMatrix::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkxAnimatedMatrix::SerializeTo(nemesis::Serializer& serializer) const
{
    auto cur_ver = serializer.GetContentsVersion();

    if (cur_ver > HavokVersion::HK_660)
    {
        nemesis::hkReferencedObject::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8
    }

    if (cur_ver > HavokVersion::HK_2012_2_0)
    {
        if (m_matrices_1.IsEmpty())
        {
            for (size_t i = 0; i < m_matrices_0.GetSize(); ++i)
            {
                auto& matrice = m_matrices_0[i];
                Combine(m_matrices_1, matrice.GetCol0());
                Combine(m_matrices_1, matrice.GetCol1());
                Combine(m_matrices_1, matrice.GetCol2());
                Combine(m_matrices_1, matrice.GetCol3());
            }
        }

        serializer.WriteObject("matrices", m_matrices_1); // offset: 8/16 size: 12/16 align: 4/8
    }
    else
    {
        if (m_matrices_0.IsEmpty())
        {
            for (size_t i = 0; i < m_matrices_1.GetSize();)
            {
                auto& mtrc = m_matrices_0.PushBack(new nemesis::hkMatrix4());
                i          = Combine(mtrc.GetCol0(), m_matrices_1, i);

                if (i >= m_matrices_1.GetSize()) break;

                i = Combine(mtrc.GetCol1(), m_matrices_1, i);

                if (i >= m_matrices_1.GetSize()) break;

                i = Combine(mtrc.GetCol2(), m_matrices_1, i);

                if (i >= m_matrices_1.GetSize()) break;

                i = Combine(mtrc.GetCol3(), m_matrices_1, i);
            }
        }

        serializer.WriteObject("matrices", m_matrices_0); // offset: 8/16 size: 12/16 align: 4/8
    }

    serializer.WriteValue("hint", m_hint);       // offset: 20/32 size: 1/1 align: 1/1
    serializer.Pad(serializer.GetPointerSize()); // offset: 21/33 size: 3/7
    // class size: 24/40 align: 4/8
}

void nemesis::hkxAnimatedMatrix::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    auto cur_ver = deserializer.GetContentsVersion();

    if (cur_ver > HavokVersion::HK_660)
    {
        nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    }

    if (cur_ver > HavokVersion::HK_2012_2_0)
    {
        m_matrices_0.Clear();
        deserializer.ReadObject("matrices", m_matrices_1);
    }
    else
    {
        m_matrices_1.Clear();
        deserializer.ReadObject("matrices", m_matrices_0); // offset: 8/16 size: 12/16 align: 4/8
    }

    deserializer.ReadValue("hint", m_hint);          // offset: 20/32 size: 1/1 align: 1/1
    deserializer.Pad(deserializer.GetPointerSize()); // offset: 21/33 size: 3/7
    // class size: 24/40 align: 4/8
}
