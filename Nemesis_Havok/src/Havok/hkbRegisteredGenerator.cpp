#include "Havok/hkbRegisteredGenerator.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbRegisteredGenerator,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbRegisteredGenerator::Class; });

nemesis::hkbRegisteredGenerator::hkbRegisteredGenerator() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkbBindable(Class)
{
}

const nemesis::hkClass* nemesis::hkbRegisteredGenerator::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbRegisteredGenerator::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkbBindable::SerializeTo(serializer);    // offset: 0/0 size: 28/48 align: 4/8

    if (serializer.GetContentsVersion() <= HavokVersion::HK_2010_2_0)
    {
        serializer.WriteObject("generator", m_generator); // offset: 28/48 size: 4/8 align: 4/8
        serializer.Pad(16);                               // offset: 32/56 size: 0/8

        serializer.WriteValue("relativePosition",
                              m_relativePosition); // offset: 32/64 size: 16/16 align: 16/16
        serializer.WriteValue("relativeDirection",
                              m_relativeDirection); // offset: 48/80 size: 16/16 align: 16/16
    }

    // class size: 64/96 align: 16/16
}

void nemesis::hkbRegisteredGenerator::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkbBindable::DeserializeFrom(deserializer); // offset: 0/0 size: 28/48 align: 4/8

    if (deserializer.GetContentsVersion() <= HavokVersion::HK_2010_2_0)
    {
        deserializer.ReadObject("generator", m_generator); // offset: 28/48 size: 4/8 align: 4/8
        deserializer.Pad(16);                              // offset: 32/56 size: 0/8

        deserializer.ReadValue("relativePosition",
                               m_relativePosition); // offset: 32/64 size: 16/16 align: 16/16
        deserializer.ReadValue("relativeDirection",
                               m_relativeDirection); // offset: 48/80 size: 16/16 align: 16/16
    }

    // class size: 64/96 align: 16/16
}
