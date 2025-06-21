#include "Havok/Base/hkpShapePhantom.h"

nemesis::hkpShapePhantom::hkpShapePhantom(const nemesis::hkClass& cls) noexcept
    : nemesis::HavokObject(cls.GetSignature())
    , nemesis::hkpPhantom(cls)
{
}

void nemesis::hkpShapePhantom::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpPhantom::SerializeTo(serializer); // offset: 0/0 size: 164/240 align: 4/8
    serializer.Pad(16);                           // offset: 164/240 size: 12/0

    serializer.WriteObject("motionState", m_motionState); // offset: 176/240 size: 176/176 align: 16/16
    // class size: 352/416 align: 16/16
}

void nemesis::hkpShapePhantom::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpPhantom::DeserializeFrom(deserializer); // offset: 0/0 size: 164/240 align: 4/8
    deserializer.Pad(16);                               // offset: 164/240 size: 12/0

    deserializer.ReadObject("motionState", m_motionState); // offset: 176/240 size: 176/176 align: 16/16
    // class size: 352/416 align: 16/16
}
