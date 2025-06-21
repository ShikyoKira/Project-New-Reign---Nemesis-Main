#include "Havok/hkbBlendCurveUtils.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbBlendCurveUtils,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbBlendCurveUtils::Class; });

nemesis::hkbBlendCurveUtils::hkbBlendCurveUtils() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkbBlendCurveUtils::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbBlendCurveUtils::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("", static_cast<unsigned char>(0x00)); // offset: 0/0 size: 1/1 align: 1/1
    // class size: 1/1 align: 1/1
}

void nemesis::hkbBlendCurveUtils::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.AssertValue<unsigned char>({0x00}); // offset: 0/0 size: 1/1 align: 1/1
    // class size: 1/1 align: 1/1
}
