#include "Havok/hkxSparselyAnimatedEnum.h"

REGISTER_HAVOK_POINTER_SOURCE(hkxSparselyAnimatedEnum,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkxSparselyAnimatedEnum::Class; });

nemesis::hkxSparselyAnimatedEnum::hkxSparselyAnimatedEnum() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkxSparselyAnimatedInt(Class)
{
}

const nemesis::hkClass* nemesis::hkxSparselyAnimatedEnum::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkxSparselyAnimatedEnum::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkxSparselyAnimatedInt::SerializeTo(serializer); // offset: 0/0 size: 32/48 align: 4/8

    if (serializer.GetContentsVersion() > HavokVersion::HK_660)
    {
        serializer.WriteObject("enum", m_enum); // offset: 32/48 size: 4/8 align: 4/8
    }
    else
    {
        serializer.WriteObject("type", m_type);
    }

    // class size: 36/56 align: 4/8
}

void nemesis::hkxSparselyAnimatedEnum::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkxSparselyAnimatedInt::DeserializeFrom(deserializer); // offset: 0/0 size: 32/48 align: 4/8

    if (deserializer.GetContentsVersion() > HavokVersion::HK_660)
    {
        deserializer.ReadObject("enum", m_enum); // offset: 32/48 size: 4/8 align: 4/8
    }
    else
    {
        deserializer.ReadObject("type", m_type);
    }

    // class size: 36/56 align: 4/8
}
