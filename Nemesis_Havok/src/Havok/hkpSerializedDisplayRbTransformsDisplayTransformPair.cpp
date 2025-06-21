#include "Havok/hkpSerializedDisplayRbTransformsDisplayTransformPair.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpSerializedDisplayRbTransformsDisplayTransformPair,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpSerializedDisplayRbTransformsDisplayTransformPair::Class; });

nemesis::hkpSerializedDisplayRbTransformsDisplayTransformPair::hkpSerializedDisplayRbTransformsDisplayTransformPair() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkpSerializedDisplayRbTransformsDisplayTransformPair::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpSerializedDisplayRbTransformsDisplayTransformPair::SerializeTo(
    nemesis::Serializer& serializer) const
{
    serializer.WriteObject("rb", m_rb); // offset: 0/0 size: 4/8 align: 4/8
    serializer.Pad(16);                 // offset: 4/8 size: 12/8

    serializer.WriteValue("localToDisplay", m_localToDisplay); // offset: 16/16 size: 64/64 align: 16/16
    // class size: 80/80 align: 16/16
}

void nemesis::hkpSerializedDisplayRbTransformsDisplayTransformPair::DeserializeFrom(
    nemesis::Deserializer& deserializer)
{
    deserializer.ReadObject("rb", m_rb); // offset: 0/0 size: 4/8 align: 4/8
    deserializer.Pad(16);                // offset: 4/8 size: 12/8

    deserializer.ReadValue("localToDisplay", m_localToDisplay); // offset: 16/16 size: 64/64 align: 16/16
    // class size: 80/80 align: 16/16
}
