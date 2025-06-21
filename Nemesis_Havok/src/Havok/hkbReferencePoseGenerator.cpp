#include "Havok/hkbReferencePoseGenerator.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbReferencePoseGenerator,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbReferencePoseGenerator::Class; });

nemesis::hkbReferencePoseGenerator::hkbReferencePoseGenerator() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkbGenerator(Class)
{
}

const nemesis::hkClass* nemesis::hkbReferencePoseGenerator::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbReferencePoseGenerator::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkbGenerator::SerializeTo(serializer); // offset: 0/0 size: 40/72 align: 4/8
    serializer.Pad(serializer.GetPointerSize());

    serializer.WriteSerializeIgnoredObject("skeleton", m_skeleton); // offset: 40/72 size: 4/8 align: 4/8
    // class size: 44/80 align: 4/8
}

void nemesis::hkbReferencePoseGenerator::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkbGenerator::DeserializeFrom(deserializer); // offset: 0/0 size: 40/72 align: 4/8
    deserializer.Pad(deserializer.GetPointerSize());

    deserializer.ReadObject("skeleton", m_skeleton); // offset: 40/72 size: 4/8 align: 4/8
    // class size: 44/80 align: 4/8
}
