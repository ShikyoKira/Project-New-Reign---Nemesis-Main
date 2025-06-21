#include "Havok/hkPostFinishAttribute.h"

REGISTER_HAVOK_POINTER_SOURCE(hkPostFinishAttribute,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkPostFinishAttribute::Class; });

nemesis::hkPostFinishAttribute::hkPostFinishAttribute() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkPostFinishAttribute::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkPostFinishAttribute::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteSerializeIgnoredObject("postFinishFunction",
                                           m_postFinishFunction); // offset: 0/0 size: 4/8 align: 4/8
    // class size: 4/8 align: 4/8
}

void nemesis::hkPostFinishAttribute::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadObject("postFinishFunction", m_postFinishFunction); // offset: 0/0 size: 4/8 align: 4/8
    // class size: 4/8 align: 4/8
}
