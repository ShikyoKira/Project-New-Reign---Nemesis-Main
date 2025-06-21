#include "Havok/hkpDefaultConvexListFilter.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpDefaultConvexListFilter,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpDefaultConvexListFilter::Class; });

nemesis::hkpDefaultConvexListFilter::hkpDefaultConvexListFilter() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkpConvexListFilter(Class)
{
}

const nemesis::hkClass* nemesis::hkpDefaultConvexListFilter::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpDefaultConvexListFilter::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpConvexListFilter::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8
    // class size: 8/16 align: 4/8
}

void nemesis::hkpDefaultConvexListFilter::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpConvexListFilter::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    // class size: 8/16 align: 4/8
}
