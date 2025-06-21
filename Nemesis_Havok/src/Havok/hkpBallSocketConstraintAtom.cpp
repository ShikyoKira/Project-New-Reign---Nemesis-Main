#include "Havok/hkpBallSocketConstraintAtom.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpBallSocketConstraintAtom,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpBallSocketConstraintAtom::Class; });

nemesis::hkpBallSocketConstraintAtom::hkpBallSocketConstraintAtom() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkpConstraintAtom(Class)
{
}

const nemesis::hkClass* nemesis::hkpBallSocketConstraintAtom::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpBallSocketConstraintAtom::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpConstraintAtom::SerializeTo(serializer); // offset: 0/0 size: 2/2 align: 2/2

    auto cur_ver = serializer.GetContentsVersion();

    if (cur_ver > HavokVersion::HK_2010_1_0)
    {
        serializer.WriteValue("solvingMethod",
                              m_solvingMethod);                    // offset: 2/2 size: 1/1 align: 1/1
        serializer.WriteValue("bodiesToNotify", m_bodiesToNotify); // offset: 3/3 size: 1/1 align: 1/1
        serializer.WriteValue("velocityStabilizationFactor",
                              m_velocityStabilizationFactor); // offset: 4/4 size: 1/1 align: 1/1
    }
    else
    {
        serializer.WriteValue("bodiesToNotify", m_bodiesToNotify);
        serializer.WriteValue("stabilizationFactor", m_velocityStabilizationFactor);
    }

    if (cur_ver > HavokVersion::HK_2011_3_0)
    {
        serializer.WriteValue("enableLinearImpulseLimit", m_enableLinearImpulseLimit);
        serializer.Skip(2);

        serializer.WriteValue("breachImpulse", m_maxImpulse);
    }
    else
    {
        serializer.Pad(4); // offset: 5/5 size: 3/3
        serializer.WriteValue("maxImpulse",
                              m_maxImpulse); // offset: 8/8 size: 4/4 align: 4/4
    }

    serializer.WriteValue(cur_ver > HavokVersion::HK_2010_1_0 ? "inertiaStabilizationFactor"
                                                              : "solverStabilizationFactor",
                          m_inertiaStabilizationFactor); // offset: 12/12 size: 4/4 align: 4/4
    // class size: 16/16 align: 4/4
}

void nemesis::hkpBallSocketConstraintAtom::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpConstraintAtom::DeserializeFrom(deserializer); // offset: 0/0 size: 2/2 align: 2/2

    auto cur_ver = deserializer.GetContentsVersion();

    if (cur_ver > HavokVersion::HK_2010_1_0)
    {
        deserializer.ReadValue("solvingMethod", m_solvingMethod);   // offset: 2/2 size: 1/1 align: 1/1
        deserializer.ReadValue("bodiesToNotify", m_bodiesToNotify); // offset: 3/3 size: 1/1 align: 1/1
        deserializer.ReadValue("velocityStabilizationFactor",
                               m_velocityStabilizationFactor); // offset: 4/4 size: 1/1 align: 1/1
    }
    else
    {
        deserializer.ReadValue("bodiesToNotify", m_bodiesToNotify);
        deserializer.ReadValue("stabilizationFactor", m_velocityStabilizationFactor);
    }

    if (cur_ver > HavokVersion::HK_2011_3_0)
    {
        deserializer.ReadValue("enableLinearImpulseLimit", m_enableLinearImpulseLimit);
        deserializer.Skip(2);

        deserializer.ReadValue("breachImpulse", m_maxImpulse);
    }
    else
    {
        deserializer.Pad(4); // offset: 5/5 size: 3/3
        deserializer.ReadValue("maxImpulse",
                               m_maxImpulse); // offset: 8/8 size: 4/4 align: 4/4
    }

    deserializer.ReadValue(cur_ver > HavokVersion::HK_2010_1_0 ? "inertiaStabilizationFactor"
                                                               : "solverStabilizationFactor",
                           m_inertiaStabilizationFactor); // offset: 12/12 size: 4/4 align: 4/4
    // class size: 16/16 align: 4/4
}
