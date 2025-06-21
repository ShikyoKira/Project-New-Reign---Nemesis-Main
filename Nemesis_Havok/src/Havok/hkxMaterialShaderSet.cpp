#include "Havok/hkxMaterialShaderSet.h"

REGISTER_HAVOK_POINTER_SOURCE(hkxMaterialShaderSet,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkxMaterialShaderSet::Class; });

nemesis::hkxMaterialShaderSet::hkxMaterialShaderSet() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkxMaterialShaderSet::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkxMaterialShaderSet::SerializeTo(nemesis::Serializer& serializer) const
{
    if (serializer.GetContentsVersion() > HavokVersion::HK_660)
    {
        nemesis::hkReferencedObject::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8
    }

    serializer.WriteObject("shaders", m_shaders); // offset: 8/16 size: 12/16 align: 4/8
    // class size: 20/32 align: 4/8
}

void nemesis::hkxMaterialShaderSet::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    if (deserializer.GetContentsVersion() > HavokVersion::HK_660)
    {
        nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    }

    deserializer.ReadObject("shaders", m_shaders); // offset: 8/16 size: 12/16 align: 4/8
    // class size: 20/32 align: 4/8
}
