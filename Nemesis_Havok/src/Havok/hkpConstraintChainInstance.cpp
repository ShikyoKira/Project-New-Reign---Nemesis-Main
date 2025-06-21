#include "Havok/hkpConstraintChainInstance.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpConstraintChainInstance,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpConstraintChainInstance::Class; });

nemesis::hkpConstraintChainInstance::hkpConstraintChainInstance() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkpConstraintInstance(Class)
{
}

const nemesis::hkClass* nemesis::hkpConstraintChainInstance::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpConstraintChainInstance::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpConstraintInstance::SerializeTo(serializer);      // offset: 0/0 size: 56/112 align: 4/8
    serializer.WriteObject("chainedEntities", m_chainedEntities); // offset: 56/112 size: 12/16 align: 4/8
    serializer.WriteObject("action", m_action);                   // offset: 68/128 size: 4/8 align: 4/8

    if (serializer.GetContentsVersion() > HavokVersion::HK_2011_3_0)
    {
        serializer.WriteValue("chainConnectedness", m_chainConnectedness);
    }

    // class size: 72/136 align: 4/8
}

void nemesis::hkpConstraintChainInstance::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpConstraintInstance::DeserializeFrom(deserializer); // offset: 0/0 size: 56/112 align: 4/8
    deserializer.ReadObject("chainedEntities", m_chainedEntities); // offset: 56/112 size: 12/16 align: 4/8
    deserializer.ReadObject("action", m_action);                   // offset: 68/128 size: 4/8 align: 4/8

    if (deserializer.GetContentsVersion() > HavokVersion::HK_2011_3_0)
    {
        deserializer.ReadValue("chainConnectedness", m_chainConnectedness);
    }

    // class size: 72/136 align: 4/8
}
