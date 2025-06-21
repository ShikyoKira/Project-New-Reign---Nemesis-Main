#include "Havok/hkxSparselyAnimatedBool.h"

REGISTER_HAVOK_POINTER_SOURCE(hkxSparselyAnimatedBool,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkxSparselyAnimatedBool::Class; });

nemesis::hkxSparselyAnimatedBool::hkxSparselyAnimatedBool() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkxSparselyAnimatedBool::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkxSparselyAnimatedBool::SerializeTo(nemesis::Serializer& serializer) const
{
    if (serializer.GetContentsVersion() > HavokVersion::HK_660)
    {
        nemesis::hkReferencedObject::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8
    }

    serializer.WriteObject("bools", m_bools); // offset: 8/16 size: 12/16 align: 4/8
    serializer.WriteObject("times", m_times); // offset: 20/32 size: 12/16 align: 4/8
    // class size: 32/48 align: 4/8
}

void nemesis::hkxSparselyAnimatedBool::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    if (deserializer.GetContentsVersion() > HavokVersion::HK_660)
    {
        nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    }

    deserializer.ReadObject("bools", m_bools); // offset: 8/16 size: 12/16 align: 4/8
    deserializer.ReadObject("times", m_times); // offset: 20/32 size: 12/16 align: 4/8
    // class size: 32/48 align: 4/8
}
