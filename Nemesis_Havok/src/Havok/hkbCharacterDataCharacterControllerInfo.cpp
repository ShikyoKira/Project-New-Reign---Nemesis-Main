#include "Havok/hkbCharacterDataCharacterControllerInfo.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbCharacterDataCharacterControllerInfo,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbCharacterDataCharacterControllerInfo::Class; });

nemesis::hkbCharacterDataCharacterControllerInfo::hkbCharacterDataCharacterControllerInfo() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkbCharacterDataCharacterControllerInfo::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbCharacterDataCharacterControllerInfo::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("capsuleHeight", m_capsuleHeight);             // offset: 0/0 size: 4/4 align: 4/4
    serializer.WriteValue("capsuleRadius", m_capsuleRadius);             // offset: 4/4 size: 4/4 align: 4/4
    serializer.WriteValue("collisionFilterInfo", m_collisionFilterInfo); // offset: 8/8 size: 4/4 align: 4/4
    serializer.Pad(serializer.GetPointerSize());                         // offset: 12/12 size: 0/4

    serializer.WriteObject("characterControllerCinfo",
                           m_characterControllerCinfo); // offset: 12/16 size: 4/8 align: 4/8
    // class size: 16/24 align: 4/8
}

void nemesis::hkbCharacterDataCharacterControllerInfo::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("capsuleHeight", m_capsuleHeight);             // offset: 0/0 size: 4/4 align: 4/4
    deserializer.ReadValue("capsuleRadius", m_capsuleRadius);             // offset: 4/4 size: 4/4 align: 4/4
    deserializer.ReadValue("collisionFilterInfo", m_collisionFilterInfo); // offset: 8/8 size: 4/4 align: 4/4
    deserializer.Pad(deserializer.GetPointerSize());                      // offset: 12/12 size: 0/4

    deserializer.ReadObject("characterControllerCinfo",
                            m_characterControllerCinfo); // offset: 12/16 size: 4/8 align: 4/8
    // class size: 16/24 align: 4/8
}
