#include "Havok/hkbClientCharacterState.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbClientCharacterState,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbClientCharacterState::Class; });

nemesis::hkbClientCharacterState::hkbClientCharacterState() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkbClientCharacterState::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbClientCharacterState::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkReferencedObject::SerializeTo(serializer);             // offset: 0/0 size: 8/16 align: 4/8
    serializer.WriteObject("deformableSkinIds", m_deformableSkinIds); // offset: 8/16 size: 12/16 align: 4/8
    serializer.WriteObject("rigidSkinIds", m_rigidSkinIds);           // offset: 20/32 size: 12/16 align: 4/8
    serializer.WriteObject("externalEventIds", m_externalEventIds);   // offset: 32/48 size: 12/16 align: 4/8
    serializer.WriteObject("auxiliaryInfo", m_auxiliaryInfo);         // offset: 44/64 size: 12/16 align: 4/8
    serializer.WriteObject("activeEventIds", m_activeEventIds);       // offset: 56/80 size: 12/16 align: 4/8
    serializer.WriteObject("activeVariableIds", m_activeVariableIds); // offset: 68/96 size: 12/16 align: 4/8
    serializer.WriteValue("characterId", m_characterId);              // offset: 80/112 size: 8/8 align: 8/8
    serializer.WriteValue("instanceName", m_instanceName);            // offset: 88/120 size: 4/8 align: 4/8
    serializer.WriteValue("templateName", m_templateName);            // offset: 92/128 size: 4/8 align: 4/8
    serializer.WriteValue("fullPathToProject", m_fullPathToProject);  // offset: 96/136 size: 4/8 align: 4/8

    if (serializer.GetContentsVersion() > HavokVersion::HK_2010_2_0)
    {
        serializer.WriteValue("localScriptsPath", m_localScriptsPath);
        serializer.WriteValue("remoteScriptsPath", m_remoteScriptsPath);
    }

    serializer.WriteObject("behaviorData", m_behaviorData);           // offset: 100/144 size: 4/8 align: 4/8
    serializer.WriteObject("behaviorInternalState",
                           m_behaviorInternalState); // offset: 104/152 size: 4/8 align: 4/8
    serializer.WriteSerializeIgnoredObject(
        "nodeIdToInternalStateMap", m_nodeIdToInternalStateMap); // offset: 108/160 size: 4/8 align: 4/8
    serializer.WriteValue("visible", m_visible);                 // offset: 112/168 size: 1/1 align: 1/1
    serializer.Skip(3);                                          // offset: 113/169 size: 3/3

    serializer.WriteValue("elapsedSimulationTime",
                          m_elapsedSimulationTime); // offset: 116/172 size: 4/4 align: 4/4
    serializer.WriteObject("skeleton", m_skeleton); // offset: 120/176 size: 4/8 align: 4/8
    serializer.Pad(16);                             // offset: 124/184 size: 4/8

    serializer.WriteValue("worldFromModel", m_worldFromModel);  // offset: 128/192 size: 48/48 align: 16/16
    serializer.WriteObject("poseModelSpace", m_poseModelSpace); // offset: 176/240 size: 12/16 align: 4/8
    serializer.WriteObject("rigidAttachmentTransforms",
                           m_rigidAttachmentTransforms); // offset: 188/256 size: 12/16 align: 4/8
    serializer.Pad(16);                                  // offset: 200/272 size: 8/0
    // class size: 208/272 align: 16/16
}

void nemesis::hkbClientCharacterState::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkReferencedObject::DeserializeFrom(deserializer);        // offset: 0/0 size: 8/16 align: 4/8
    deserializer.ReadObject("deformableSkinIds", m_deformableSkinIds); // offset: 8/16 size: 12/16 align: 4/8
    deserializer.ReadObject("rigidSkinIds", m_rigidSkinIds);           // offset: 20/32 size: 12/16 align: 4/8
    deserializer.ReadObject("externalEventIds", m_externalEventIds);   // offset: 32/48 size: 12/16 align: 4/8
    deserializer.ReadObject("auxiliaryInfo", m_auxiliaryInfo);         // offset: 44/64 size: 12/16 align: 4/8
    deserializer.ReadObject("activeEventIds", m_activeEventIds);       // offset: 56/80 size: 12/16 align: 4/8
    deserializer.ReadObject("activeVariableIds", m_activeVariableIds); // offset: 68/96 size: 12/16 align: 4/8
    deserializer.ReadValue("characterId", m_characterId);              // offset: 80/112 size: 8/8 align: 8/8
    deserializer.ReadValue("instanceName", m_instanceName);            // offset: 88/120 size: 4/8 align: 4/8
    deserializer.ReadValue("templateName", m_templateName);            // offset: 92/128 size: 4/8 align: 4/8
    deserializer.ReadValue("fullPathToProject", m_fullPathToProject);  // offset: 96/136 size: 4/8 align: 4/8

    if (deserializer.GetContentsVersion() > HavokVersion::HK_2010_2_0)
    {
        deserializer.ReadValue("localScriptsPath", m_localScriptsPath);
        deserializer.ReadValue("remoteScriptsPath", m_remoteScriptsPath);
    }

    deserializer.ReadObject("behaviorData", m_behaviorData);           // offset: 100/144 size: 4/8 align: 4/8
    deserializer.ReadObject("behaviorInternalState",
                            m_behaviorInternalState); // offset: 104/152 size: 4/8 align: 4/8
    deserializer.ReadObject("nodeIdToInternalStateMap",
                            m_nodeIdToInternalStateMap); // offset: 108/160 size: 4/8 align: 4/8
    deserializer.ReadValue("visible", m_visible);        // offset: 112/168 size: 1/1 align: 1/1
    deserializer.Skip(3);                                // offset: 113/169 size: 3/3

    deserializer.ReadValue("elapsedSimulationTime",
                           m_elapsedSimulationTime); // offset: 116/172 size: 4/4 align: 4/4
    deserializer.ReadObject("skeleton", m_skeleton); // offset: 120/176 size: 4/8 align: 4/8
    deserializer.Pad(16);                            // offset: 124/184 size: 4/8

    deserializer.ReadValue("worldFromModel", m_worldFromModel);  // offset: 128/192 size: 48/48 align: 16/16
    deserializer.ReadObject("poseModelSpace", m_poseModelSpace); // offset: 176/240 size: 12/16 align: 4/8
    deserializer.ReadObject("rigidAttachmentTransforms",
                            m_rigidAttachmentTransforms); // offset: 188/256 size: 12/16 align: 4/8
    deserializer.Pad(16);                                 // offset: 200/272 size: 8/0
    // class size: 208/272 align: 16/16
}
