#include "Havok/hkbCharacterSteppedInfo.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbCharacterSteppedInfo,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbCharacterSteppedInfo::Class; });

nemesis::hkbCharacterSteppedInfo::hkbCharacterSteppedInfo() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkbCharacterSteppedInfo::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbCharacterSteppedInfo::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkReferencedObject::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8
    serializer.WriteValue("characterId", m_characterId);  // offset: 8/16 size: 8/8 align: 8/8
    serializer.WriteValue("deltaTime", m_deltaTime);      // offset: 16/24 size: 4/4 align: 4/4
    serializer.Pad(16);                                   // offset: 20/28 size: 12/4

    serializer.WriteValue("worldFromModel", m_worldFromModel);  // offset: 32/32 size: 48/48 align: 16/16
    serializer.WriteObject("poseModelSpace", m_poseModelSpace); // offset: 80/80 size: 12/16 align: 4/8
    serializer.WriteObject("rigidAttachmentTransforms",
                           m_rigidAttachmentTransforms); // offset: 92/96 size: 12/16 align: 4/8
    serializer.Pad(16);                                  // offset: 104/112 size: 8/0
    // class size: 112/112 align: 16/16
}

void nemesis::hkbCharacterSteppedInfo::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    deserializer.ReadValue("characterId", m_characterId);       // offset: 8/16 size: 8/8 align: 8/8
    deserializer.ReadValue("deltaTime", m_deltaTime);           // offset: 16/24 size: 4/4 align: 4/4
    deserializer.Pad(16);                                       // offset: 20/28 size: 12/4

    deserializer.ReadValue("worldFromModel", m_worldFromModel);  // offset: 32/32 size: 48/48 align: 16/16
    deserializer.ReadObject("poseModelSpace", m_poseModelSpace); // offset: 80/80 size: 12/16 align: 4/8
    deserializer.ReadObject("rigidAttachmentTransforms",
                            m_rigidAttachmentTransforms); // offset: 92/96 size: 12/16 align: 4/8
    deserializer.Pad(16);                                 // offset: 104/112 size: 8/0
    // class size: 112/112 align: 16/16
}
