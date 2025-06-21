#include "Havok/Base/hkpPhantom.h"

nemesis::hkpPhantom::hkpPhantom(const nemesis::hkClass& cls) noexcept
    : nemesis::HavokObject(cls.GetSignature())
    , nemesis::hkpWorldObject(cls)
{
}

void nemesis::hkpPhantom::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpWorldObject::SerializeTo(serializer); // offset: 0/0 size: 140/208 align: 4/8
    serializer.WriteSerializeIgnoredObject("overlapListeners",
                                           m_overlapListeners); // offset: 140/208 size: 12/16 align: 4/8
    serializer.WriteSerializeIgnoredObject("phantomListeners",
                                           m_phantomListeners); // offset: 152/224 size: 12/16 align: 4/8
    // class size: 164/240 align: 4/8
}

void nemesis::hkpPhantom::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpWorldObject::DeserializeFrom(deserializer);          // offset: 0/0 size: 140/208 align: 4/8
    deserializer.ReadObject("overlapListeners", m_overlapListeners); // offset: 140/208 size: 12/16 align: 4/8
    deserializer.ReadObject("phantomListeners", m_phantomListeners); // offset: 152/224 size: 12/16 align: 4/8
    // class size: 164/240 align: 4/8
}
