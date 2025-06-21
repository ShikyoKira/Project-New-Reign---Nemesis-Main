#include "Havok/hkbBehaviorReferenceGenerator.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbBehaviorReferenceGenerator,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbBehaviorReferenceGenerator::Class; });

nemesis::hkbBehaviorReferenceGenerator::hkbBehaviorReferenceGenerator() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkbGenerator(Class)
{
}

const nemesis::hkClass* nemesis::hkbBehaviorReferenceGenerator::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbBehaviorReferenceGenerator::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkbGenerator::SerializeTo(serializer); // offset: 0/0 size: 40/72 align: 4/8
    serializer.Pad(serializer.GetPointerSize());

    serializer.WriteValue("behaviorName", m_behaviorName);          // offset: 40/72 size: 4/8 align: 4/8
    serializer.WriteSerializeIgnoredObject("behavior", m_behavior); // offset: 44/80 size: 4/8 align: 4/8
    // class size: 48/88 align: 4/8
}

void nemesis::hkbBehaviorReferenceGenerator::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkbGenerator::DeserializeFrom(deserializer); // offset: 0/0 size: 40/72 align: 4/8
    deserializer.Pad(deserializer.GetPointerSize());

    deserializer.ReadValue("behaviorName", m_behaviorName); // offset: 40/72 size: 4/8 align: 4/8
    deserializer.ReadObject("behavior", m_behavior);        // offset: 44/80 size: 4/8 align: 4/8
    // class size: 48/88 align: 4/8
}
