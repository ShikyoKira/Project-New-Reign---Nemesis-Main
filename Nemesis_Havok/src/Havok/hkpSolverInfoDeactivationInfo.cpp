#include "Havok/hkpSolverInfoDeactivationInfo.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpSolverInfoDeactivationInfo,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpSolverInfoDeactivationInfo::Class; });

nemesis::hkpSolverInfoDeactivationInfo::hkpSolverInfoDeactivationInfo() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkpSolverInfoDeactivationInfo::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpSolverInfoDeactivationInfo::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("linearVelocityThresholdInv",
                          m_linearVelocityThresholdInv); // offset: 0/0 size: 4/4 align: 4/4
    serializer.WriteValue("angularVelocityThresholdInv",
                          m_angularVelocityThresholdInv); // offset: 4/4 size: 4/4 align: 4/4
    serializer.WriteValue("slowObjectVelocityMultiplier",
                          m_slowObjectVelocityMultiplier); // offset: 8/8 size: 4/4 align: 4/4
    serializer.WriteValue("relativeSleepVelocityThreshold",
                          m_relativeSleepVelocityThreshold); // offset: 12/12 size: 4/4 align: 4/4
    serializer.WriteValue("maxDistSqrd", m_maxDistSqrd);     // offset: 16/16 size: 8/8 align: 4/4

    if (serializer.GetContentsVersion() > HavokVersion::HK_2010_2_0)
    {
        serializer.WriteValue("maxRotSqrd", m_maxRotSqrd_1);
    }
    else
    {
        serializer.WriteValue("maxRotSqrd", m_maxRotSqrd_0); // offset: 24/24 size: 4/4 align: 2/2
    }

    // class size: 28/28 align: 4/4
}

void nemesis::hkpSolverInfoDeactivationInfo::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("linearVelocityThresholdInv",
                           m_linearVelocityThresholdInv); // offset: 0/0 size: 4/4 align: 4/4
    deserializer.ReadValue("angularVelocityThresholdInv",
                           m_angularVelocityThresholdInv); // offset: 4/4 size: 4/4 align: 4/4
    deserializer.ReadValue("slowObjectVelocityMultiplier",
                           m_slowObjectVelocityMultiplier); // offset: 8/8 size: 4/4 align: 4/4
    deserializer.ReadValue("relativeSleepVelocityThreshold",
                           m_relativeSleepVelocityThreshold); // offset: 12/12 size: 4/4 align: 4/4
    deserializer.ReadValue("maxDistSqrd", m_maxDistSqrd);     // offset: 16/16 size: 8/8 align: 4/4

    if (deserializer.GetContentsVersion() > HavokVersion::HK_2010_2_0)
    {
        deserializer.ReadValue("maxRotSqrd", m_maxRotSqrd_1);

        for (size_t i = 0; i < m_maxRotSqrd_0.size(); i++)
        {
            m_maxRotSqrd_0[i] = m_maxRotSqrd_1[i];
        }
    }
    else
    {
        deserializer.ReadValue("maxRotSqrd", m_maxRotSqrd_0); // offset: 24/24 size: 4/4 align: 2/2

        for (size_t i = 0; i < m_maxRotSqrd_0.size(); i++)
        {
            m_maxRotSqrd_1[i] = m_maxRotSqrd_0[i].AsFloat();
        }
    }

    // class size: 28/28 align: 4/4
}
