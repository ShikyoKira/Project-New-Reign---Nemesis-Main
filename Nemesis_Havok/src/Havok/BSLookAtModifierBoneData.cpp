#include "Havok/BSLookAtModifierBoneData.h"

REGISTER_HAVOK_POINTER_SOURCE(BSLookAtModifierBoneData,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::BSLookAtModifierBoneData::Class; });

nemesis::BSLookAtModifierBoneData::BSLookAtModifierBoneData() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::BSLookAtModifierBoneData::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::BSLookAtModifierBoneData::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("index", m_index); // offset: 0/0 size: 2/2 align: 2/2
    serializer.Skip(14);                     // offset: 2/2 size: 14/14

    serializer.WriteValue("fwdAxisLS", m_fwdAxisLS);                 // offset: 16/16 size: 16/16 align: 16/16
    serializer.WriteValue("limitAngleDegrees", m_limitAngleDegrees); // offset: 32/32 size: 4/4 align: 4/4
    serializer.WriteValue("onGain", m_onGain);                       // offset: 36/36 size: 4/4 align: 4/4
    serializer.WriteValue("offGain", m_offGain);                     // offset: 40/40 size: 4/4 align: 4/4
    serializer.WriteValue("enabled", m_enabled);                     // offset: 44/44 size: 1/1 align: 1/1
    serializer.Skip(3);                                              // offset: 45/45 size: 3/3

    serializer.WriteSerializeIgnoredValue("currentFwdAxisLS",
                                          m_currentFwdAxisLS); // offset: 48/48 size: 16/16 align: 16/16
    // class size: 64/64 align: 16/16
}

void nemesis::BSLookAtModifierBoneData::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("index", m_index); // offset: 0/0 size: 2/2 align: 2/2
    deserializer.Skip(14);                    // offset: 2/2 size: 14/14

    deserializer.ReadValue("fwdAxisLS", m_fwdAxisLS); // offset: 16/16 size: 16/16 align: 16/16
    deserializer.ReadValue("limitAngleDegrees", m_limitAngleDegrees); // offset: 32/32 size: 4/4 align: 4/4
    deserializer.ReadValue("onGain", m_onGain);                       // offset: 36/36 size: 4/4 align: 4/4
    deserializer.ReadValue("offGain", m_offGain);                     // offset: 40/40 size: 4/4 align: 4/4
    deserializer.ReadValue("enabled", m_enabled);                     // offset: 44/44 size: 1/1 align: 1/1
    deserializer.Skip(3);                                             // offset: 45/45 size: 3/3

    deserializer.ReadValue("currentFwdAxisLS", m_currentFwdAxisLS); // offset: 48/48 size: 16/16 align: 16/16
    // class size: 64/64 align: 16/16
}
