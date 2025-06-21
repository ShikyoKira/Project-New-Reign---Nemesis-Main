#include "Havok/hkxSparselyAnimatedStringStringType.h"

REGISTER_HAVOK_POINTER_SOURCE(hkxSparselyAnimatedStringStringType,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkxSparselyAnimatedStringStringType::Class; });

nemesis::hkxSparselyAnimatedStringStringType::hkxSparselyAnimatedStringStringType() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkxSparselyAnimatedStringStringType::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkxSparselyAnimatedStringStringType::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("string", m_string); // offset: 0/0 size: 4/8 align: 4/8
    // class size: 4/8 align: 4/8
}

void nemesis::hkxSparselyAnimatedStringStringType::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("string", m_string); // offset: 0/0 size: 4/8 align: 4/8
    // class size: 4/8 align: 4/8
}
