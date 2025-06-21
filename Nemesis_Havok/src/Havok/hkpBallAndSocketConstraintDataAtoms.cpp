#include "Havok/hkpBallAndSocketConstraintDataAtoms.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpBallAndSocketConstraintDataAtoms,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpBallAndSocketConstraintDataAtoms::Class; });

nemesis::hkpBallAndSocketConstraintDataAtoms::hkpBallAndSocketConstraintDataAtoms() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkpBallAndSocketConstraintDataAtoms::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpBallAndSocketConstraintDataAtoms::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteObject("pivots", m_pivots); // offset: 0/0 size: 48/48 align: 16/16

    if (serializer.GetContentsVersion() > HavokVersion::HK_2010_1_0)
    {
        serializer.WriteObject("setupStabilization",
                               m_setupStabilization); // offset: 48/48 size: 16/16 align: 4/4
    }

    serializer.WriteObject("ballSocket", m_ballSocket); // offset: 64/64 size: 16/16 align: 4/4
    // class size: 80/80 align: 16/16
}

void nemesis::hkpBallAndSocketConstraintDataAtoms::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadObject("pivots", m_pivots); // offset: 0/0 size: 48/48 align: 16/16

    if (deserializer.GetContentsVersion() > HavokVersion::HK_2010_1_0)
    {
        deserializer.ReadObject("setupStabilization",
                                m_setupStabilization); // offset: 48/48 size: 16/16 align: 4/4
    }

    deserializer.ReadObject("ballSocket", m_ballSocket); // offset: 64/64 size: 16/16 align: 4/4
    // class size: 80/80 align: 16/16
}
