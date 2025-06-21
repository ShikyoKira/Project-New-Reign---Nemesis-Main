#include "Havok/hkpSerializedDisplayRbTransforms.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpSerializedDisplayRbTransforms,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpSerializedDisplayRbTransforms::Class; });

nemesis::hkpSerializedDisplayRbTransforms::hkpSerializedDisplayRbTransforms() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkpSerializedDisplayRbTransforms::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpSerializedDisplayRbTransforms::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkReferencedObject::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8
    serializer.WriteObject("transforms", m_transforms);   // offset: 8/16 size: 12/16 align: 4/8
    // class size: 20/32 align: 4/8
}

void nemesis::hkpSerializedDisplayRbTransforms::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    deserializer.ReadObject("transforms", m_transforms);        // offset: 8/16 size: 12/16 align: 4/8
    // class size: 20/32 align: 4/8
}
