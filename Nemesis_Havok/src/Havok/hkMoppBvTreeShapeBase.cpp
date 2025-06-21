#include "Havok/hkMoppBvTreeShapeBase.h"

REGISTER_HAVOK_POINTER_SOURCE(hkMoppBvTreeShapeBase,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkMoppBvTreeShapeBase::Class; });

nemesis::hkMoppBvTreeShapeBase::hkMoppBvTreeShapeBase() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkpBvTreeShape(Class)
{
}

const nemesis::hkClass* nemesis::hkMoppBvTreeShapeBase::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkMoppBvTreeShapeBase::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpBvTreeShape::SerializeTo(serializer);               // offset: 0/0 size: 20/40 align: 4/8
    serializer.WriteObject("code", m_code);                         // offset: 20/40 size: 4/8 align: 4/8
    serializer.WriteSerializeIgnoredObject("moppData", m_moppData); // offset: 24/48 size: 4/8 align: 4/8
    serializer.WriteSerializeIgnoredValue("moppDataSize",
                                          m_moppDataSize); // offset: 28/56 size: 4/4 align: 4/4
    serializer.Pad(16);                                    // offset: 32/60 size: 0/4

    serializer.WriteSerializeIgnoredValue("codeInfoCopy",
                                          m_codeInfoCopy); // offset: 32/64 size: 16/16 align: 16/16
    // class size: 48/80 align: 16/16
}

void nemesis::hkMoppBvTreeShapeBase::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpBvTreeShape::DeserializeFrom(deserializer); // offset: 0/0 size: 20/40 align: 4/8
    deserializer.ReadObject("code", m_code);                // offset: 20/40 size: 4/8 align: 4/8
    deserializer.ReadObject("moppData", m_moppData);        // offset: 24/48 size: 4/8 align: 4/8
    deserializer.ReadValue("moppDataSize", m_moppDataSize); // offset: 28/56 size: 4/4 align: 4/4
    deserializer.Pad(16);                                   // offset: 32/60 size: 0/4

    deserializer.ReadValue("codeInfoCopy", m_codeInfoCopy); // offset: 32/64 size: 16/16 align: 16/16
    // class size: 48/80 align: 16/16
}
