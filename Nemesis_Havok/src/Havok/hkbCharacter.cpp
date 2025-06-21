#include "Havok/hkbCharacter.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbCharacter,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbCharacter::Class; });

nemesis::hkbCharacter::hkbCharacter() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkbCharacter::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbCharacter::SerializeTo(nemesis::Serializer& serializer) const
{
    auto cur_ver = serializer.GetContentsVersion();

    nemesis::hkReferencedObject::SerializeTo(serializer);           // offset: 0/0 size: 8/16 align: 4/8

    if (cur_ver > HavokVersion::HK_710)
    {
        serializer.WriteObject("nearbyCharacters", m_nearbyCharacters); // offset: 8/16 size: 12/16 align: 4/8

        if (cur_ver > HavokVersion::HK_2011_1_0)
        {
            serializer.WriteValue("userData", m_userData);
        }

        serializer.WriteValue("currentLod", m_currentLod);              // offset: 20/32 size: 2/2 align: 2/2
    }

    serializer.WriteSerializeIgnoredValue("numTracksInLod",
                                          m_numTracksInLod); // offset: 22/34 size: 2/2 align: 2/2
    serializer.Pad(serializer.GetPointerSize());             // offset: 24/36 size: 0/4

    if (cur_ver > HavokVersion::HK_710)
    {
        serializer.WriteValue("name", m_name); // offset: 24/40 size: 4/8 align: 4/8
    }

    serializer.WriteSerializeIgnoredObject("ragdollDriver",
                                           m_ragdollDriver); // offset: 28/48 size: 4/8 align: 4/8
    serializer.WriteSerializeIgnoredObject("characterControllerDriver",
                                           m_characterControllerDriver); // offset: 32/56 size: 4/8 align: 4/8
    serializer.WriteSerializeIgnoredObject("footIkDriver",
                                           m_footIkDriver); // offset: 36/64 size: 4/8 align: 4/8
    serializer.WriteSerializeIgnoredObject("handIkDriver",
                                           m_handIkDriver);   // offset: 40/72 size: 4/8 align: 4/8

    if (cur_ver > HavokVersion::HK_710)
    {
        serializer.WriteObject("setup", m_setup);                 // offset: 44/80 size: 4/8 align: 4/8
        serializer.WriteObject("behaviorGraph", m_behaviorGraph); // offset: 48/88 size: 4/8 align: 4/8
        serializer.WriteObject("projectData", m_projectData);     // offset: 52/96 size: 4/8 align: 4/8
    }

    serializer.WriteSerializeIgnoredObject("animationBindingSet",
                                           m_animationBindingSet); // offset: 56/104 size: 4/8 align: 4/8
    serializer.WriteSerializeIgnoredObject("raycastInterface",
                                           m_raycastInterface);         // offset: 60/112 size: 4/8 align: 4/8
    serializer.WriteSerializeIgnoredObject("world", m_world);           // offset: 64/120 size: 4/8 align: 4/8
    serializer.WriteSerializeIgnoredObject("eventQueue", m_eventQueue); // offset: 68/128 size: 4/8 align: 4/8
    serializer.WriteSerializeIgnoredObject("worldFromModel",
                                           m_worldFromModel);         // offset: 72/136 size: 4/8 align: 4/8
    serializer.WriteSerializeIgnoredObject("poseLocal", m_poseLocal); // offset: 76/144 size: 4/8 align: 4/8
    serializer.WriteSerializeIgnoredValue("numPoseLocal",
                                          m_numPoseLocal); // offset: 80/152 size: 4/4 align: 4/4

    if (cur_ver > HavokVersion::HK_2013_2_0)
    {
        serializer.WriteValue("capabilities", m_capabilities);
        serializer.WriteValue("effectiveCapabilities", m_effectiveCapabilities);
    }

    serializer.WriteSerializeIgnoredValue("deleteWorldFromModel",
                                          m_deleteWorldFromModel); // offset: 84/156 size: 1/1 align: 1/1
    serializer.WriteSerializeIgnoredValue("deletePoseLocal",
                                          m_deletePoseLocal); // offset: 85/157 size: 1/1 align: 1/1
    serializer.Pad(serializer.GetPointerSize());              // offset: 86/158 size: 2/2
    // class size: 88/160 align: 4/8
}

void nemesis::hkbCharacter::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    auto cur_ver = deserializer.GetContentsVersion();

    nemesis::hkReferencedObject::DeserializeFrom(deserializer);      // offset: 0/0 size: 8/16 align: 4/8

    if (cur_ver > HavokVersion::HK_710)
    {
        deserializer.ReadObject("nearbyCharacters",
                                m_nearbyCharacters);        // offset: 8/16 size: 12/16 align: 4/8

        if (cur_ver > HavokVersion::HK_2011_1_0)
        {
            deserializer.ReadValue("userData", m_userData);
        }

        deserializer.ReadValue("currentLod", m_currentLod); // offset: 20/32 size: 2/2 align: 2/2
    }

    deserializer.ReadValue("numTracksInLod", m_numTracksInLod);      // offset: 22/34 size: 2/2 align: 2/2
    deserializer.Pad(deserializer.GetPointerSize());                 // offset: 24/36 size: 0/4

    if (cur_ver > HavokVersion::HK_710)
    {
        deserializer.ReadValue("name", m_name); // offset: 24/40 size: 4/8 align: 4/8
    }

    deserializer.ReadObject("ragdollDriver", m_ragdollDriver); // offset: 28/48 size: 4/8 align: 4/8
    deserializer.ReadObject("characterControllerDriver",
                            m_characterControllerDriver);      // offset: 32/56 size: 4/8 align: 4/8
    deserializer.ReadObject("footIkDriver", m_footIkDriver);   // offset: 36/64 size: 4/8 align: 4/8
    deserializer.ReadObject("handIkDriver", m_handIkDriver);   // offset: 40/72 size: 4/8 align: 4/8

    if (cur_ver > HavokVersion::HK_710)
    {
        deserializer.ReadObject("setup", m_setup);                 // offset: 44/80 size: 4/8 align: 4/8
        deserializer.ReadObject("behaviorGraph", m_behaviorGraph); // offset: 48/88 size: 4/8 align: 4/8
        deserializer.ReadObject("projectData", m_projectData);     // offset: 52/96 size: 4/8 align: 4/8
    }

    deserializer.ReadObject("animationBindingSet",
                            m_animationBindingSet);                  // offset: 56/104 size: 4/8 align: 4/8
    deserializer.ReadObject("raycastInterface", m_raycastInterface); // offset: 60/112 size: 4/8 align: 4/8
    deserializer.ReadObject("world", m_world);                       // offset: 64/120 size: 4/8 align: 4/8
    deserializer.ReadObject("eventQueue", m_eventQueue);             // offset: 68/128 size: 4/8 align: 4/8
    deserializer.ReadObject("worldFromModel", m_worldFromModel);     // offset: 72/136 size: 4/8 align: 4/8
    deserializer.ReadObject("poseLocal", m_poseLocal);               // offset: 76/144 size: 4/8 align: 4/8
    deserializer.ReadValue("numPoseLocal",
                           m_numPoseLocal); // offset: 80/152 size: 4/4 align: 4/4

    if (cur_ver > HavokVersion::HK_2013_2_0)
    {
        deserializer.ReadValue("capabilities", m_capabilities);
        deserializer.ReadValue("effectiveCapabilities", m_effectiveCapabilities);
    }

    deserializer.ReadValue("deleteWorldFromModel",
                           m_deleteWorldFromModel);               // offset: 84/156 size: 1/1 align: 1/1
    deserializer.ReadValue("deletePoseLocal", m_deletePoseLocal); // offset: 85/157 size: 1/1 align: 1/1
    deserializer.Skip(2);                                         // offset: 86/158 size: 2/2
    // class size: 84/160 align: 4/8
}
