#include "Havok/hkbProjectStringData.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbProjectStringData,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbProjectStringData::Class; });

nemesis::hkbProjectStringData::hkbProjectStringData() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkbProjectStringData::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbProjectStringData::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkReferencedObject::SerializeTo(serializer);               // offset: 0/0 size: 8/16 align: 4/8
    serializer.WriteObject("animationFilenames", m_animationFilenames); // offset: 8/16 size: 12/16 align: 4/8
    serializer.WriteObject("behaviorFilenames", m_behaviorFilenames); // offset: 20/32 size: 12/16 align: 4/8
    serializer.WriteObject("characterFilenames",
                           m_characterFilenames);            // offset: 32/48 size: 12/16 align: 4/8
    serializer.WriteObject("eventNames", m_eventNames);      // offset: 44/64 size: 12/16 align: 4/8
    serializer.WriteValue("animationPath", m_animationPath); // offset: 56/80 size: 4/8 align: 4/8
    serializer.WriteValue("behaviorPath", m_behaviorPath);   // offset: 60/88 size: 4/8 align: 4/8
    serializer.WriteValue("characterPath", m_characterPath); // offset: 64/96 size: 4/8 align: 4/8

    auto cur_ver = serializer.GetContentsVersion();

    if (cur_ver > HavokVersion::HK_710)
    {
        serializer.WriteValue("fullPathToSource", m_fullPathToSource); // offset: 68/104 size: 4/8 align: 4/8
    }

    serializer.WriteSerializeIgnoredValue("rootPath", m_rootPath); // offset: 72/112 size: 4/8 align: 4/8

    if (cur_ver > HavokVersion::HK_2010_2_0)
    {
        serializer.WriteValue("scriptsPath", m_scriptsPath);
    }

    // class size: 76/120 align: 4/8
}

void nemesis::hkbProjectStringData::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    deserializer.ReadObject("animationFilenames",
                            m_animationFilenames);                     // offset: 8/16 size: 12/16 align: 4/8
    deserializer.ReadObject("behaviorFilenames", m_behaviorFilenames); // offset: 20/32 size: 12/16 align: 4/8
    deserializer.ReadObject("characterFilenames",
                            m_characterFilenames);            // offset: 32/48 size: 12/16 align: 4/8
    deserializer.ReadObject("eventNames", m_eventNames);      // offset: 44/64 size: 12/16 align: 4/8
    deserializer.ReadValue("animationPath", m_animationPath); // offset: 56/80 size: 4/8 align: 4/8
    deserializer.ReadValue("behaviorPath", m_behaviorPath);   // offset: 60/88 size: 4/8 align: 4/8
    deserializer.ReadValue("characterPath", m_characterPath); // offset: 64/96 size: 4/8 align: 4/8

    auto cur_ver = deserializer.GetContentsVersion();

    if (cur_ver > HavokVersion::HK_710)
    {
        deserializer.ReadValue("fullPathToSource", m_fullPathToSource); // offset: 68/104 size: 4/8 align: 4/8
    }

    deserializer.ReadValue("rootPath", m_rootPath); // offset: 72/112 size: 4/8 align: 4/8

    if (cur_ver > HavokVersion::HK_2010_2_0)
    {
        deserializer.ReadValue("scriptsPath", m_scriptsPath);
    }

    // class size: 76/120 align: 4/8
}
