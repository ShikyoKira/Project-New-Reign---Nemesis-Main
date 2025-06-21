#include "Havok/hkaMeshBindingMapping.h"

REGISTER_HAVOK_POINTER_SOURCE(hkaMeshBindingMapping,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkaMeshBindingMapping::Class; });

nemesis::hkaMeshBindingMapping::hkaMeshBindingMapping() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkaMeshBindingMapping::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkaMeshBindingMapping::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteObject("mapping", m_mapping); // offset: 0/0 size: 12/16 align: 4/8
    // class size: 12/16 align: 4/8
}

void nemesis::hkaMeshBindingMapping::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadObject("mapping", m_mapping); // offset: 0/0 size: 12/16 align: 4/8
    // class size: 12/16 align: 4/8
}
