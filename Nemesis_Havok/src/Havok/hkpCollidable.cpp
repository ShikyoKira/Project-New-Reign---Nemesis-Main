#include "Havok/hkpCollidable.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpCollidable,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpCollidable::Class; });

nemesis::hkpCollidable::hkpCollidable() noexcept
    : nemesis::hkpCollidable(Class)
{
}

nemesis::hkpCollidable::hkpCollidable(const nemesis::hkClass& cls) noexcept
    : nemesis::HavokObject(cls.GetSignature())
    , nemesis::hkpCdBody(cls)
{
}

const nemesis::hkClass* nemesis::hkpCollidable::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpCollidable::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpCdBody::SerializeTo(serializer);                         // offset: 0/0 size: 16/32 align: 4/8
    serializer.WriteSerializeIgnoredValue("ownerOffset", m_ownerOffset); // offset: 16/32 size: 1/1 align: 1/1
    serializer.WriteValue("forceCollideOntoPpu", m_forceCollideOntoPpu); // offset: 17/33 size: 1/1 align: 1/1
    serializer.WriteSerializeIgnoredValue("shapeSizeOnSpu",
                                          m_shapeSizeOnSpu);        // offset: 18/34 size: 2/2 align: 2/2
    serializer.WriteObject("broadPhaseHandle", m_broadPhaseHandle); // offset: 20/36 size: 12/12 align: 4/4
    serializer.WriteSerializeIgnoredObject("boundingVolumeData",
                                           m_boundingVolumeData); // offset: 32/48 size: 44/56 align: 4/8
    serializer.WriteValue("allowedPenetrationDepth",
                          m_allowedPenetrationDepth); // offset: 76/104 size: 4/4 align: 4/4
    serializer.Pad(serializer.GetPointerSize());      // offset: 80/108 size: 0/4
    // class size: 80/112 align: 4/8
}

void nemesis::hkpCollidable::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpCdBody::DeserializeFrom(deserializer);    // offset: 0/0 size: 16/32 align: 4/8
    deserializer.ReadValue("ownerOffset", m_ownerOffset); // offset: 16/32 size: 1/1 align: 1/1
    deserializer.ReadValue("forceCollideOntoPpu",
                           m_forceCollideOntoPpu);                   // offset: 17/33 size: 1/1 align: 1/1
    deserializer.ReadValue("shapeSizeOnSpu", m_shapeSizeOnSpu);      // offset: 18/34 size: 2/2 align: 2/2
    deserializer.ReadObject("broadPhaseHandle", m_broadPhaseHandle); // offset: 20/36 size: 12/12 align: 4/4
    deserializer.ReadObject("boundingVolumeData",
                            m_boundingVolumeData); // offset: 32/48 size: 44/56 align: 4/8
    deserializer.ReadValue("allowedPenetrationDepth",
                           m_allowedPenetrationDepth); // offset: 76/104 size: 4/4 align: 4/4
    deserializer.Pad(deserializer.GetPointerSize());   // offset: 80/108 size: 0/4
    // class size: 80/112 align: 4/8
}
