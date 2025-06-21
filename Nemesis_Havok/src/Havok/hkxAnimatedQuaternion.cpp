#include "Havok/hkxAnimatedQuaternion.h"

REGISTER_HAVOK_POINTER_SOURCE(hkxAnimatedQuaternion,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkxAnimatedQuaternion::Class; });

void nemesis::hkxAnimatedQuaternion::Combine(nemesis::hkArray<float>& quaternions,
                                             const nemesis::hkQuaternion& qrtn)
{
    quaternions.PushBack(new float(qrtn.GetX()));
    quaternions.PushBack(new float(qrtn.GetY()));
    quaternions.PushBack(new float(qrtn.GetZ()));
    quaternions.PushBack(new float(qrtn.GetR()));
}

size_t nemesis::hkxAnimatedQuaternion::Combine(nemesis::hkQuaternion& qrtn,
                                               const nemesis::hkArray<float>& quaternions,
                                               size_t index)
{
    qrtn.SetX(quaternions[index++]);

    if (index >= quaternions.GetSize()) return index;

    qrtn.SetY(quaternions[index++]);

    if (index >= quaternions.GetSize()) return index;

    qrtn.SetZ(quaternions[index++]);

    if (index >= quaternions.GetSize()) return index;

    qrtn.SetR(quaternions[index++]);
    return index;
}

nemesis::hkxAnimatedQuaternion::hkxAnimatedQuaternion() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkxAnimatedQuaternion::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkxAnimatedQuaternion::SerializeTo(nemesis::Serializer& serializer) const
{
    auto cur_ver = serializer.GetContentsVersion();

    if (cur_ver > HavokVersion::HK_660)
    {
        nemesis::hkReferencedObject::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8
    }

    if (cur_ver > HavokVersion::HK_2012_2_0)
    {
        if (m_quaternions_1.IsEmpty())
        {
            for (size_t i = 0; i < m_quaternions_0.GetSize(); ++i)
            {
                Combine(m_quaternions_1, m_quaternions_0[i]);
            }
        }

        serializer.WriteObject("quaternions", m_quaternions_1);
    }
    else
    {
        if (m_quaternions_0.IsEmpty())
        {
            for (size_t i = 0; i < m_quaternions_1.GetSize();)
            {
                i = Combine(m_quaternions_0.PushBack(new nemesis::hkQuaternion()), m_quaternions_1, i);
            }
        }

        serializer.WriteObject("quaternions", m_quaternions_0); // offset: 8/16 size: 12/16 align: 4/8
    }
    // class size: 20/32 align: 4/8
}

void nemesis::hkxAnimatedQuaternion::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    auto cur_ver = deserializer.GetContentsVersion();

    if (cur_ver > HavokVersion::HK_660)
    {
        nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    }

    if (cur_ver > HavokVersion::HK_2012_2_0)
    {
        m_quaternions_0.Clear();
        deserializer.ReadObject("quaternions", m_quaternions_1);
    }
    else
    {
        m_quaternions_1.Clear();
        deserializer.ReadObject("quaternions", m_quaternions_0); // offset: 8/16 size: 12/16 align: 4/8
    }

    // class size: 20/32 align: 4/8
}
