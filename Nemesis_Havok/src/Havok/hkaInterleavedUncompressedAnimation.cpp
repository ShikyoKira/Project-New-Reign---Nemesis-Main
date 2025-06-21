#include "Havok/hkaInterleavedUncompressedAnimation.h"

REGISTER_HAVOK_POINTER_SOURCE(hkaInterleavedUncompressedAnimation,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkaInterleavedUncompressedAnimation::Class; });

nemesis::hkaInterleavedUncompressedAnimation::hkaInterleavedUncompressedAnimation() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkaAnimation(Class)
{
}

const nemesis::hkClass* nemesis::hkaInterleavedUncompressedAnimation::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkaInterleavedUncompressedAnimation::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkaAnimation::SerializeTo(serializer);     // offset: 0/0 size: 40/56 align: 4/8
    serializer.WriteObject("transforms", m_transforms); // offset: 40/56 size: 12/16 align: 4/8
    serializer.WriteObject("floats", m_floats);         // offset: 52/72 size: 12/16 align: 4/8
    // class size: 64/88 align: 4/8
}

void nemesis::hkaInterleavedUncompressedAnimation::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkaAnimation::DeserializeFrom(deserializer); // offset: 0/0 size: 40/56 align: 4/8
    deserializer.ReadObject("transforms", m_transforms);  // offset: 40/56 size: 12/16 align: 4/8
    deserializer.ReadObject("floats", m_floats);          // offset: 52/72 size: 12/16 align: 4/8
    // class size: 64/88 align: 4/8
}
