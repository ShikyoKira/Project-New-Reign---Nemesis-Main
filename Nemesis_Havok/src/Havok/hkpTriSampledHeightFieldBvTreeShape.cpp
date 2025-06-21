#include "Havok/hkpTriSampledHeightFieldBvTreeShape.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpTriSampledHeightFieldBvTreeShape,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpTriSampledHeightFieldBvTreeShape::Class; });

nemesis::hkpTriSampledHeightFieldBvTreeShape::hkpTriSampledHeightFieldBvTreeShape() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkpBvTreeShape(Class)
{
}

const nemesis::hkClass* nemesis::hkpTriSampledHeightFieldBvTreeShape::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpTriSampledHeightFieldBvTreeShape::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpBvTreeShape::SerializeTo(serializer);                // offset: 0/0 size: 20/40 align: 4/8
    serializer.WriteObject("childContainer", m_childContainer);      // offset: 20/40 size: 12/16 align: 4/8
    serializer.WriteSerializeIgnoredValue("childSize", m_childSize); // offset: 32/56 size: 4/4 align: 4/4
    serializer.WriteValue("wantAabbRejectionTest",
                          m_wantAabbRejectionTest); // offset: 36/60 size: 1/1 align: 1/1

    if (serializer.GetContentsVersion() <= HavokVersion::HK_2011_3_0)
    {
        serializer.WriteValue("padding", m_padding); // offset: 37/61 size: 12/12 align: 1/1
    }

    serializer.Pad(serializer.GetPointerSize()); // offset: 49/73 size: 3/7
    // class size: 52/80 align: 4/8
}

void nemesis::hkpTriSampledHeightFieldBvTreeShape::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpBvTreeShape::DeserializeFrom(deserializer);      // offset: 0/0 size: 20/40 align: 4/8
    deserializer.ReadObject("childContainer", m_childContainer); // offset: 20/40 size: 12/16 align: 4/8
    deserializer.ReadValue("childSize", m_childSize);            // offset: 32/56 size: 4/4 align: 4/4
    deserializer.ReadValue("wantAabbRejectionTest",
                           m_wantAabbRejectionTest); // offset: 36/60 size: 1/1 align: 1/1

    if (deserializer.GetContentsVersion() <= HavokVersion::HK_2011_3_0)
    {
        deserializer.ReadValue("padding", m_padding); // offset: 37/61 size: 12/12 align: 1/1
    }

    deserializer.Pad(deserializer.GetPointerSize()); // offset: 49/73 size: 3/7
    // class size: 52/80 align: 4/8
}
