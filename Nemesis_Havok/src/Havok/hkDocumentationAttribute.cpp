#include "Havok/hkDocumentationAttribute.h"

REGISTER_HAVOK_POINTER_SOURCE(hkDocumentationAttribute,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkDocumentationAttribute::Class; });

nemesis::hkDocumentationAttribute::hkDocumentationAttribute() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkDocumentationAttribute::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkDocumentationAttribute::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("docsSectionTag", m_docsSectionTag); // offset: 0/0 size: 4/8 align: 4/8
    // class size: 4/8 align: 4/8
}

void nemesis::hkDocumentationAttribute::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("docsSectionTag", m_docsSectionTag); // offset: 0/0 size: 4/8 align: 4/8
    // class size: 4/8 align: 4/8
}
