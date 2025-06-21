#include "Havok/hkpMeshShape.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpMeshShape,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpMeshShape::Class; });

nemesis::hkpMeshShape::hkpMeshShape() noexcept
    : nemesis::hkpMeshShape(Class)
{
}

nemesis::hkpMeshShape::hkpMeshShape(const nemesis::hkClass& cls) noexcept
    : nemesis::HavokObject(cls.GetSignature())
    , nemesis::hkpShapeCollection(cls)
{
}

const nemesis::hkClass* nemesis::hkpMeshShape::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpMeshShape::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpShapeCollection::SerializeTo(serializer); // offset: 0/0 size: 24/48 align: 4/8
    serializer.Pad(16);                                   // offset: 24/48 size: 8/0

    serializer.WriteValue("scaling", m_scaling); // offset: 32/48 size: 16/16 align: 16/16
    serializer.WriteValue("numBitsForSubpartIndex",
                          m_numBitsForSubpartIndex); // offset: 48/64 size: 4/4 align: 4/4
    serializer.Pad(serializer.GetPointerSize());     // offset: 52/68 size: 0/4

    serializer.WriteObject("subparts", m_subparts);       // offset: 52/72 size: 12/16 align: 4/8
    serializer.WriteObject("weldingInfo", m_weldingInfo); // offset: 64/88 size: 12/16 align: 4/8
    serializer.WriteValue("weldingType", m_weldingType);  // offset: 76/104 size: 1/1 align: 1/1
    serializer.Skip(3);                                   // offset: 77/105 size: 3/3

    serializer.WriteValue("radius", m_radius); // offset: 80/108 size: 4/4 align: 4/4
    serializer.WriteValue("pad", m_pad);       // offset: 84/112 size: 12/12 align: 4/4
    serializer.Pad(16);                        // offset: 96/124 size: 0/4
    // class size: 96/128 align: 16/16
}

void nemesis::hkpMeshShape::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpShapeCollection::DeserializeFrom(deserializer); // offset: 0/0 size: 24/48 align: 4/8
    deserializer.Pad(16);                                       // offset: 24/48 size: 8/0

    deserializer.ReadValue("scaling", m_scaling); // offset: 32/48 size: 16/16 align: 16/16
    deserializer.ReadValue("numBitsForSubpartIndex",
                           m_numBitsForSubpartIndex); // offset: 48/64 size: 4/4 align: 4/4
    deserializer.Pad(deserializer.GetPointerSize());  // offset: 52/68 size: 0/4

    deserializer.ReadObject("subparts", m_subparts);       // offset: 52/72 size: 12/16 align: 4/8
    deserializer.ReadObject("weldingInfo", m_weldingInfo); // offset: 64/88 size: 12/16 align: 4/8
    deserializer.ReadValue("weldingType", m_weldingType);  // offset: 76/104 size: 1/1 align: 1/1
    deserializer.Skip(3);                                  // offset: 77/105 size: 3/3

    deserializer.ReadValue("radius", m_radius); // offset: 80/108 size: 4/4 align: 4/4
    deserializer.ReadValue("pad", m_pad);       // offset: 84/112 size: 12/12 align: 4/4
    deserializer.Pad(16);                       // offset: 96/124 size: 0/4
    // class size: 96/128 align: 16/16
}
