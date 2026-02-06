#include "Havok/hkpEntity.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpEntity,
                              [](nemesis::HavokVersion version) { return &nemesis::hkpEntity::Class; });

nemesis::hkpEntity::hkpEntity() noexcept
    : nemesis::hkpEntity(Class)
{
}

nemesis::hkpEntity::hkpEntity(const nemesis::hkClass& cls) noexcept
    : nemesis::HavokObject(cls.GetSignature())
    , nemesis::hkpWorldObject(cls)
{
}

const nemesis::hkClass* nemesis::hkpEntity::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpEntity::SerializeTo(nemesis::Serializer& serializer) const
{
    unsigned int padding_size = serializer.GetPointerSize();

    nemesis::hkpWorldObject::SerializeTo(serializer); // offset: 0/0 size: 140/208 align: 4/8
    serializer.WriteObject("material", m_material);   // offset: 140/208 size: 12/12 align: 4/4
    serializer.Pad(padding_size);                     // offset: 152/220 size: 0/4

    serializer.WriteSerializeIgnoredObject(
        "limitContactImpulseUtilAndFlag",
        m_limitContactImpulseUtilAndFlag);                         // offset: 152/224 size: 4/8 align: 4/8
    serializer.WriteValue("damageMultiplier", m_damageMultiplier); // offset: 156/232 size: 4/4 align: 4/4
    serializer.Pad(padding_size);                                  // offset: 160/236 size: 0/4

    serializer.WriteSerializeIgnoredObject("breakableBody",
                                           m_breakableBody);           // offset: 160/240 size: 4/8 align: 4/8
    serializer.WriteSerializeIgnoredValue("solverData", m_solverData); // offset: 164/248 size: 4/4 align: 4/4
    serializer.WriteValue("storageIndex", m_storageIndex);             // offset: 168/252 size: 2/2 align: 2/2

    auto cur_ver = serializer.GetContentsVersion();

    serializer.WriteValue(cur_ver <= HavokVersion::HK_660 ? "processContactCallbackDelay"
                                                          : "contactPointCallbackDelay",
                          m_contactPointCallbackDelay); // offset: 170/254 size: 2/2 align: 2/2

    serializer.WriteSerializeIgnoredObject("constraintsMaster",
                                           m_constraintsMaster); // offset: 172/256 size: 8/16 align: 4/8
    serializer.WriteSerializeIgnoredObject("constraintsSlave",
                                           m_constraintsSlave); // offset: 180/272 size: 12/16 align: 4/8
    serializer.WriteSerializeIgnoredObject("constraintRuntime",
                                           m_constraintRuntime); // offset: 192/288 size: 12/16 align: 4/8
    serializer.WriteSerializeIgnoredObject("simulationIsland",
                                           m_simulationIsland);  // offset: 204/304 size: 4/8 align: 4/8
    serializer.WriteValue("autoRemoveLevel", m_autoRemoveLevel); // offset: 208/312 size: 1/1 align: 1/1

    if (cur_ver > HavokVersion::HK_660)
    {
        serializer.WriteValue("numShapeKeysInContactPointProperties",
                              m_numShapeKeysInContactPointProperties); // offset: 209/313 size: 1/1 align: 1/1
        serializer.WriteValue("responseModifierFlags",
                              m_responseModifierFlags); // offset: 210/314 size: 1/1 align: 1/1
        serializer.Skip(1);                             // offset: 211/315 size: 1/1
    }
    else
    {
        serializer.WriteValue("numUserDatasInContactPointProperties", m_numShapeKeysInContactPointProperties);
        serializer.Skip(2);
    }

    serializer.WriteValue("uid", m_uid); // offset: 212/316 size: 4/4 align: 4/4
    serializer.WriteObject("spuCollisionCallback",
                           m_spuCollisionCallback); // offset: 216/320 size: 8/16 align: 4/8
    serializer.WriteObject("motion", m_motion);     // offset: 224/336 size: 288/320 align: 16/16
    serializer.WriteSerializeIgnoredObject("contactListeners",
                                           m_contactListeners);   // offset: 512/656 size: 8/16 align: 4/8
    serializer.WriteSerializeIgnoredObject("actions", m_actions); // offset: 520/672 size: 8/16 align: 4/8
    serializer.WriteObject("localFrame", m_localFrame);           // offset: 528/688 size: 4/8 align: 4/8
    serializer.WriteSerializeIgnoredObject("extendedListeners",
                                           m_extendedListeners); // offset: 532/696 size: 4/8 align: 4/8

    if (serializer.GetContentsVersion() > HavokVersion::HK_710)
    {
        serializer.WriteValue("npData", m_npData); // offset: 536/704 size: 4/4 align: 4/4
    }

    serializer.Pad(16); // offset: 540/708 size: 4/12
    // class size: 544/720 align: 16/16
}

void nemesis::hkpEntity::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    unsigned int padding_size = deserializer.GetPointerSize();

    nemesis::hkpWorldObject::DeserializeFrom(deserializer); // offset: 0/0 size: 140/208 align: 4/8
    deserializer.ReadObject("material", m_material);        // offset: 140/208 size: 12/12 align: 4/4
    deserializer.Pad(padding_size);                         // offset: 152/220 size: 0/4

    deserializer.ReadObject("limitContactImpulseUtilAndFlag",
                            m_limitContactImpulseUtilAndFlag);      // offset: 152/224 size: 4/8 align: 4/8
    deserializer.ReadValue("damageMultiplier", m_damageMultiplier); // offset: 156/232 size: 4/4 align: 4/4
    deserializer.Pad(padding_size);                                 // offset: 160/236 size: 0/4

    deserializer.ReadObject("breakableBody", m_breakableBody); // offset: 160/240 size: 4/8 align: 4/8
    deserializer.ReadValue("solverData", m_solverData);        // offset: 164/248 size: 4/4 align: 4/4
    deserializer.ReadValue("storageIndex", m_storageIndex);    // offset: 168/252 size: 2/2 align: 2/2

    auto cur_ver = deserializer.GetContentsVersion();

    deserializer.ReadValue(cur_ver <= HavokVersion::HK_660 ? "processContactCallbackDelay"
                                                           : "contactPointCallbackDelay",
                           m_contactPointCallbackDelay); // offset: 170/254 size: 2/2 align: 2/2
    deserializer.ReadObject("constraintsMaster",
                            m_constraintsMaster);                    // offset: 172/256 size: 8/16 align: 4/8
    deserializer.ReadObject("constraintsSlave", m_constraintsSlave); // offset: 180/272 size: 12/16 align: 4/8
    deserializer.ReadObject("constraintRuntime",
                            m_constraintRuntime);                    // offset: 192/288 size: 12/16 align: 4/8
    deserializer.ReadObject("simulationIsland", m_simulationIsland); // offset: 204/304 size: 4/8 align: 4/8
    deserializer.ReadValue("autoRemoveLevel", m_autoRemoveLevel);    // offset: 208/312 size: 1/1 align: 1/1

    if (cur_ver > HavokVersion::HK_660)
    {
        deserializer.ReadValue(
            "numShapeKeysInContactPointProperties",
            m_numShapeKeysInContactPointProperties); // offset: 209/313 size: 1/1 align: 1/1
        deserializer.ReadValue("responseModifierFlags",
                               m_responseModifierFlags); // offset: 210/314 size: 1/1 align: 1/1
        deserializer.Skip(1);                            // offset: 211/315 size: 1/1
    }
    else
    {
        deserializer.ReadValue("numUserDatasInContactPointProperties",
                               m_numShapeKeysInContactPointProperties);
        deserializer.Skip(2);
    }

    deserializer.ReadValue("uid", m_uid); // offset: 212/316 size: 4/4 align: 4/4
    deserializer.ReadObject("spuCollisionCallback",
                            m_spuCollisionCallback); // offset: 216/320 size: 8/16 align: 4/8
    deserializer.ReadObject("motion", m_motion);     // offset: 224/336 size: 288/320 align: 16/16
    deserializer.ReadObject("contactListeners", m_contactListeners); // offset: 512/656 size: 8/16 align: 4/8
    deserializer.ReadObject("actions", m_actions);                   // offset: 520/672 size: 8/16 align: 4/8
    deserializer.ReadObject("localFrame", m_localFrame);             // offset: 528/688 size: 4/8 align: 4/8
    deserializer.ReadObject("extendedListeners", m_extendedListeners); // offset: 532/696 size: 4/8 align: 4/8

    if (deserializer.GetContentsVersion() > HavokVersion::HK_710)
    {
        deserializer.ReadValue("npData", m_npData); // offset: 536/704 size: 4/4 align: 4/4
    }

    deserializer.Pad(16); // offset: 540/708 size: 4/12
    // class size: 544/720 align: 16/16
}
