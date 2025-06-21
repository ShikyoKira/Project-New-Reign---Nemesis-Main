#include "Havok/hkbAuxiliaryNodeInfo.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbAuxiliaryNodeInfo,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbAuxiliaryNodeInfo::Class; });

nemesis::hkbAuxiliaryNodeInfo::hkbAuxiliaryNodeInfo() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkbAuxiliaryNodeInfo::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbAuxiliaryNodeInfo::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkReferencedObject::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8

    if (serializer.GetContentsVersion() > HavokVersion::HK_2011_3_0)
    {
        serializer.WriteValue("type", static_cast<unsigned short>(m_type.to_int()));
    }
    else
    {
        serializer.WriteValue("type", m_type); // offset: 8/16 size: 1/1 align: 1/1
    }

    serializer.WriteValue("depth", m_depth);     // offset: 9/17 size: 1/1 align: 1/1
    serializer.Pad(serializer.GetPointerSize()); // offset: 10/18 size: 2/6

    serializer.WriteValue("referenceBehaviorName",
                          m_referenceBehaviorName); // offset: 12/24 size: 4/8 align: 4/8
    serializer.WriteObject("selfTransitionNames",
                           m_selfTransitionNames); // offset: 16/32 size: 12/16 align: 4/8
    // class size: 28/48 align: 4/8
}

void nemesis::hkbAuxiliaryNodeInfo::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8

    if (deserializer.GetContentsVersion() > HavokVersion::HK_2011_3_0)
    {
        unsigned short itype;
        deserializer.ReadValue("type", itype);
        m_type = static_cast<unsigned char>(itype);
    }
    else
    {
        deserializer.ReadValue("type", m_type); // offset: 8/16 size: 1/1 align: 1/1
    }

    deserializer.ReadValue("depth", m_depth);        // offset: 9/17 size: 1/1 align: 1/1
    deserializer.Pad(deserializer.GetPointerSize()); // offset: 10/18 size: 2/6

    deserializer.ReadValue("referenceBehaviorName",
                           m_referenceBehaviorName); // offset: 12/24 size: 4/8 align: 4/8
    deserializer.ReadObject("selfTransitionNames",
                            m_selfTransitionNames); // offset: 16/32 size: 12/16 align: 4/8
    // class size: 28/48 align: 4/8
}
