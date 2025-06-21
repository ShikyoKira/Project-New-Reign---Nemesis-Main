#include "Havok/hkpStiffSpringConstraintDataAtoms.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpStiffSpringConstraintDataAtoms,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpStiffSpringConstraintDataAtoms::Class; });

nemesis::hkpStiffSpringConstraintDataAtoms::hkpStiffSpringConstraintDataAtoms() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkpStiffSpringConstraintDataAtoms::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpStiffSpringConstraintDataAtoms::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteObject("pivots", m_pivots); // offset: 0/0 size: 48/48 align: 16/16

    if (serializer.GetContentsVersion() > HavokVersion::HK_2011_3_0)
    {
        serializer.WriteObject("setupStabilization", m_setupStabilization);
    }

    serializer.WriteObject("spring", m_spring); // offset: 48/48 size: 8/8 align: 4/4
    serializer.Skip(8);                         // offset: 56/56 size: 8/8
    // class size: 64/64 align: 16/16
}

void nemesis::hkpStiffSpringConstraintDataAtoms::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadObject("pivots", m_pivots); // offset: 0/0 size: 48/48 align: 16/16

    if (deserializer.GetContentsVersion() > HavokVersion::HK_2011_3_0)
    {
        deserializer.ReadObject("setupStabilization", m_setupStabilization);
    }

    deserializer.ReadObject("spring", m_spring); // offset: 48/48 size: 8/8 align: 4/4
    deserializer.Skip(8);                        // offset: 56/56 size: 8/8
    // class size: 64/64 align: 16/16
}
