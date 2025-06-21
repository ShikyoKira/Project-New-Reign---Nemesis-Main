#include "Havok/hkpTriSampledHeightFieldCollection.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpTriSampledHeightFieldCollection,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpTriSampledHeightFieldCollection::Class; });

nemesis::hkpTriSampledHeightFieldCollection::hkpTriSampledHeightFieldCollection() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkpShapeCollection(Class)
{
}

const nemesis::hkClass* nemesis::hkpTriSampledHeightFieldCollection::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpTriSampledHeightFieldCollection::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpShapeCollection::SerializeTo(serializer);            // offset: 0/0 size: 24/48 align: 4/8
    serializer.WriteObject("heightfield", m_heightfield);            // offset: 24/48 size: 4/8 align: 4/8
    serializer.WriteSerializeIgnoredValue("childSize", m_childSize); // offset: 28/56 size: 4/4 align: 4/4
    serializer.WriteValue("radius", m_radius);                       // offset: 32/60 size: 4/4 align: 4/4
    serializer.WriteObject("weldingInfo", m_weldingInfo);            // offset: 36/64 size: 12/16 align: 4/8
    serializer.WriteValue("triangleExtrusion", m_triangleExtrusion); // offset: 48/80 size: 16/16 align: 16/16
    // class size: 64/96 align: 16/16
}

void nemesis::hkpTriSampledHeightFieldCollection::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpShapeCollection::DeserializeFrom(deserializer); // offset: 0/0 size: 24/48 align: 4/8
    deserializer.ReadObject("heightfield", m_heightfield);      // offset: 24/48 size: 4/8 align: 4/8
    deserializer.ReadValue("childSize", m_childSize);           // offset: 28/56 size: 4/4 align: 4/4
    deserializer.ReadValue("radius", m_radius);                 // offset: 32/60 size: 4/4 align: 4/4
    deserializer.ReadObject("weldingInfo", m_weldingInfo);      // offset: 36/64 size: 12/16 align: 4/8
    deserializer.ReadValue("triangleExtrusion",
                           m_triangleExtrusion); // offset: 48/80 size: 16/16 align: 16/16
    // class size: 64/96 align: 16/16
}
