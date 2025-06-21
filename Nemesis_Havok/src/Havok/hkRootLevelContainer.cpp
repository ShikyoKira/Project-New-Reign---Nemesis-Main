#include "Havok/hkRootLevelContainer.h"

REGISTER_HAVOK_POINTER_SOURCE(hkRootLevelContainer,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkRootLevelContainer::Class; });

nemesis::hkRootLevelContainer::hkRootLevelContainer() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkRootLevelContainer::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkRootLevelContainer::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteObject("namedVariants", m_namedVariants); // offset: 0/0 size: 12/16 align: 4/8
    // class size: 12/16 align: 4/8
}

void nemesis::hkRootLevelContainer::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadObject("namedVariants", m_namedVariants); // offset: 0/0 size: 12/16 align: 4/8
    // class size: 12/16 align: 4/8
}
