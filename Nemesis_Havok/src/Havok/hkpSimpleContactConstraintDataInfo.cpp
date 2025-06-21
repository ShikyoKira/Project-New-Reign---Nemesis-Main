#include "Havok/hkpSimpleContactConstraintDataInfo.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpSimpleContactConstraintDataInfo,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpSimpleContactConstraintDataInfo::Class; });

nemesis::hkpSimpleContactConstraintDataInfo::hkpSimpleContactConstraintDataInfo() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkpSimpleContactConstraintDataInfo::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpSimpleContactConstraintDataInfo::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("flags", m_flags); // offset: 0/0 size: 2/2 align: 16/16

    auto cur_ver = serializer.GetContentsVersion();

    if (cur_ver > HavokVersion::HK_2010_2_0)
    {
        serializer.WriteValue("biNormalAxis", m_biNormalAxis);
        serializer.WriteValue("rollingFrictionMultiplier", m_rollingFrictionMultiplier);
        serializer.WriteValue("internalData1", m_internalData1);
        serializer.WriteValue("rhsRolling", m_rhsRolling);
        serializer.WriteValue("contactRadius", m_contactRadius);
        serializer.WriteValue("data", m_data_2);
    }
    else if (cur_ver > HavokVersion::HK_2010_1_0)
    {
        serializer.WriteValue("index", m_index);                 // offset: 2/2 size: 2/2 align: 2/2
        serializer.WriteValue("internalData0", m_internalData0); // offset: 4/4 size: 4/4 align: 4/4
        serializer.WriteValue("rollingFrictionMultiplier",
                              m_rollingFrictionMultiplier);      // offset: 8/8 size: 2/2 align: 2/2
        serializer.WriteValue("internalData1", m_internalData1); // offset: 10/10 size: 2/2 align: 2/2
        serializer.WriteValue("data", m_data_1);                 // offset: 12/12 size: 20/20 align: 4/4
    }
    else
    {
        serializer.WriteValue("index", m_index);
        serializer.WriteValue("data", m_data_0);
    }

    // class size: 32/32 align: 16/16
}

void nemesis::hkpSimpleContactConstraintDataInfo::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("flags", m_flags); // offset: 0/0 size: 2/2 align: 16/16

    auto cur_ver = deserializer.GetContentsVersion();

    if (cur_ver > HavokVersion::HK_2012_2_0)
    {
        deserializer.ReadValue("biNormalAxis", m_biNormalAxis);
        deserializer.ReadValue("rollingFrictionMultiplier", m_rollingFrictionMultiplier);
        deserializer.ReadValue("internalData1", m_internalData1);
        deserializer.ReadValue("rhsRolling", m_rhsRolling);
        deserializer.ReadValue("contactRadius", m_contactRadius);
        deserializer.ReadValue("data", m_data_2);
    }
    else if (cur_ver > HavokVersion::HK_2010_1_0)
    {
        deserializer.ReadValue("index", m_index);                 // offset: 2/2 size: 2/2 align: 2/2
        deserializer.ReadValue("internalData0", m_internalData0); // offset: 4/4 size: 4/4 align: 4/4
        deserializer.ReadValue("rollingFrictionMultiplier",
                               m_rollingFrictionMultiplier);      // offset: 8/8 size: 2/2 align: 2/2
        deserializer.ReadValue("internalData1", m_internalData1); // offset: 10/10 size: 2/2 align: 2/2
    }
    else
    {
        deserializer.ReadValue("index", m_index);
        deserializer.ReadValue("data", m_data_0);
    }

    // class size: 32/32 align: 16/16
}
