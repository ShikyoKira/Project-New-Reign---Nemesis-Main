#include "Havok/hkpSetupStabilizationAtom.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpSetupStabilizationAtom,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpSetupStabilizationAtom::Class; });

nemesis::hkpSetupStabilizationAtom::hkpSetupStabilizationAtom() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkpConstraintAtom(Class)
{
}

const nemesis::hkClass* nemesis::hkpSetupStabilizationAtom::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpSetupStabilizationAtom::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpConstraintAtom::SerializeTo(serializer); // offset: 0/0 size: 2/2 align: 2/2
    serializer.WriteValue("enabled", m_enabled);         // offset: 2/2 size: 1/1 align: 1/1
    serializer.Skip(1);                                  // offset: 3/3 size: 1/1

    if (serializer.GetContentsVersion() > HavokVersion::HK_2011_3_0)
    {
        serializer.WriteValue("maxLinImpulse", m_maxLinImpulse);
        serializer.WriteValue("maxAngImpulse", m_maxAngImpulse);
        serializer.WriteValue("maxAngle", m_maxAngle);
    }
    else
    {
        serializer.WriteValue("maxAngle", m_maxAngle); // offset: 4/4 size: 4/4 align: 4/4
        serializer.WriteValue("padding", m_padding);   // offset: 8/8 size: 8/8 align: 1/1
    }

    // class size: 16/16 align: 4/4
}

void nemesis::hkpSetupStabilizationAtom::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpConstraintAtom::DeserializeFrom(deserializer); // offset: 0/0 size: 2/2 align: 2/2
    deserializer.ReadValue("enabled", m_enabled);              // offset: 2/2 size: 1/1 align: 1/1
    deserializer.Skip(1);                                      // offset: 3/3 size: 1/1

    if (deserializer.GetContentsVersion() > HavokVersion::HK_2011_3_0)
    {
        deserializer.ReadValue("maxLinImpulse", m_maxLinImpulse);
        deserializer.ReadValue("maxAngImpulse", m_maxAngImpulse);
        deserializer.ReadValue("maxAngle", m_maxAngle);
    }
    else
    {
        deserializer.ReadValue("maxAngle", m_maxAngle); // offset: 4/4 size: 4/4 align: 4/4
        deserializer.ReadValue("padding", m_padding);   // offset: 8/8 size: 8/8 align: 1/1
    }

    // class size: 16/16 align: 4/4
}
