#include "Havok/Base/hkbBindable.h"

nemesis::hkbBindable::hkbBindable(const nemesis::hkClass& cls) noexcept
    : nemesis::HavokObject(cls.GetSignature())
    , nemesis::hkReferencedObject(cls)
{
}

void nemesis::hkbBindable::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkReferencedObject::SerializeTo(serializer);               // offset: 0/0 size: 8/16 align: 4/8
    serializer.WriteObject("variableBindingSet", m_variableBindingSet); // offset: 8/16 size: 4/8 align: 4/8
    serializer.WriteSerializeIgnoredObject("cachedBindables",
                                           m_cachedBindables); // offset: 12/24 size: 12/16 align: 4/8
    serializer.WriteSerializeIgnoredValue("areBindablesCached",
                                          m_areBindablesCached); // offset: 24/40 size: 1/1 align: 1/1
    serializer.Pad(serializer.GetPointerSize());                 // offset: 25/41 size: 3/7
    // class size: 28/48 align: 4/8
}

void nemesis::hkbBindable::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkReferencedObject::DeserializeFrom(deserializer);          // offset: 0/0 size: 8/16 align: 4/8
    deserializer.ReadObject("variableBindingSet", m_variableBindingSet); // offset: 8/16 size: 4/8 align: 4/8
    deserializer.ReadObject("cachedBindables", m_cachedBindables); // offset: 12/24 size: 12/16 align: 4/8
    deserializer.ReadValue("areBindablesCached", m_areBindablesCached); // offset: 24/40 size: 1/1 align: 1/1
    deserializer.Pad(deserializer.GetPointerSize());                    // offset: 25/41 size: 3/7
    // class size: 28/48 align: 4/8
}
