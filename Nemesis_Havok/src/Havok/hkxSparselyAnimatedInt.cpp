#include "Havok/hkxSparselyAnimatedInt.h"

REGISTER_HAVOK_POINTER_SOURCE(hkxSparselyAnimatedInt,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkxSparselyAnimatedInt::Class; });

nemesis::hkxSparselyAnimatedInt::hkxSparselyAnimatedInt() noexcept
    : nemesis::hkxSparselyAnimatedInt(Class)
{
}

nemesis::hkxSparselyAnimatedInt::hkxSparselyAnimatedInt(const nemesis::hkClass& cls) noexcept
    : nemesis::HavokObject(cls.GetSignature())
    , nemesis::hkReferencedObject(cls)
{
}

const nemesis::hkClass* nemesis::hkxSparselyAnimatedInt::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkxSparselyAnimatedInt::SerializeTo(nemesis::Serializer& serializer) const
{
    if (serializer.GetContentsVersion() > HavokVersion::HK_660)
    {
        nemesis::hkReferencedObject::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8
    }

    serializer.WriteObject("ints", m_ints);   // offset: 8/16 size: 12/16 align: 4/8
    serializer.WriteObject("times", m_times); // offset: 20/32 size: 12/16 align: 4/8
    // class size: 32/48 align: 4/8
}

void nemesis::hkxSparselyAnimatedInt::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    if (deserializer.GetContentsVersion() > HavokVersion::HK_660)
    {
        nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    }

    deserializer.ReadObject("ints", m_ints);   // offset: 8/16 size: 12/16 align: 4/8
    deserializer.ReadObject("times", m_times); // offset: 20/32 size: 12/16 align: 4/8
    // class size: 32/48 align: 4/8
}
