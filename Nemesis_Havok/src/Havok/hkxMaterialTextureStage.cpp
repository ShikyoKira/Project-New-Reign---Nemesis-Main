#include "Havok/hkxMaterialTextureStage.h"

REGISTER_HAVOK_POINTER_SOURCE(hkxMaterialTextureStage,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkxMaterialTextureStage::Class; });

nemesis::hkxMaterialTextureStage::hkxMaterialTextureStage() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkxMaterialTextureStage::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkxMaterialTextureStage::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteObject("texture", m_texture);            // offset: 0/0 size: 4/8 align: 4/8
    serializer.WriteValue("usageHint", m_usageHint);         // offset: 4/8 size: 4/4 align: 4/4
    serializer.WriteValue("tcoordChannel", m_tcoordChannel); // offset: 8/12 size: 4/4 align: 4/4
    // class size: 12/16 align: 4/8
}

void nemesis::hkxMaterialTextureStage::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadObject("texture", m_texture);            // offset: 0/0 size: 4/8 align: 4/8
    deserializer.ReadValue("usageHint", m_usageHint);         // offset: 4/8 size: 4/4 align: 4/4
    deserializer.ReadValue("tcoordChannel", m_tcoordChannel); // offset: 8/12 size: 4/4 align: 4/4
    // class size: 12/16 align: 4/8
}
