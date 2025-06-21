#include "Havok/hkaFootstepAnalysisInfo.h"

REGISTER_HAVOK_POINTER_SOURCE(hkaFootstepAnalysisInfo,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkaFootstepAnalysisInfo::Class; });

nemesis::hkaFootstepAnalysisInfo::hkaFootstepAnalysisInfo() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkaFootstepAnalysisInfo::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkaFootstepAnalysisInfo::SerializeTo(nemesis::Serializer& serializer) const
{
    if (serializer.GetContentsVersion() > HavokVersion::HK_660)
    {
        nemesis::hkReferencedObject::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8
    }

    serializer.WriteValue("name", m_name, true);             // offset: 8/16 size: 12/16 align: 4/8
    serializer.WriteValue("nameStrike", m_nameStrike, true); // offset: 20/32 size: 12/16 align: 4/8
    serializer.WriteValue("nameLift", m_nameLift, true);     // offset: 32/48 size: 12/16 align: 4/8
    serializer.WriteValue("nameLock", m_nameLock, true);     // offset: 44/64 size: 12/16 align: 4/8
    serializer.WriteValue("nameUnlock", m_nameUnlock, true); // offset: 56/80 size: 12/16 align: 4/8
    serializer.WriteObject("minPos", m_minPos);              // offset: 68/96 size: 12/16 align: 4/8
    serializer.WriteObject("maxPos", m_maxPos);              // offset: 80/112 size: 12/16 align: 4/8
    serializer.WriteObject("minVel", m_minVel);              // offset: 92/128 size: 12/16 align: 4/8
    serializer.WriteObject("maxVel", m_maxVel);              // offset: 104/144 size: 12/16 align: 4/8
    serializer.WriteObject("allBonesDown", m_allBonesDown);  // offset: 116/160 size: 12/16 align: 4/8
    serializer.WriteObject("anyBonesDown", m_anyBonesDown);  // offset: 128/176 size: 12/16 align: 4/8
    serializer.WriteValue("posTol", m_posTol);               // offset: 140/192 size: 4/4 align: 4/4
    serializer.WriteValue("velTol", m_velTol);               // offset: 144/196 size: 4/4 align: 4/4
    serializer.WriteValue("duration", m_duration);           // offset: 148/200 size: 4/4 align: 4/4
    serializer.Pad(serializer.GetPointerSize());             // offset: 152/204 size: 0/4
    // class size: 152/208 align: 4/8
}

void nemesis::hkaFootstepAnalysisInfo::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    if (deserializer.GetContentsVersion() > HavokVersion::HK_660)
    {
        nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    }

    deserializer.ReadValue("name", m_name, true);               // offset: 8/16 size: 12/16 align: 4/8
    deserializer.ReadValue("nameStrike", m_nameStrike, true);   // offset: 20/32 size: 12/16 align: 4/8
    deserializer.ReadValue("nameLift", m_nameLift, true);       // offset: 32/48 size: 12/16 align: 4/8
    deserializer.ReadValue("nameLock", m_nameLock, true);       // offset: 44/64 size: 12/16 align: 4/8
    deserializer.ReadValue("nameUnlock", m_nameUnlock, true);   // offset: 56/80 size: 12/16 align: 4/8
    deserializer.ReadObject("minPos", m_minPos);                // offset: 68/96 size: 12/16 align: 4/8
    deserializer.ReadObject("maxPos", m_maxPos);                // offset: 80/112 size: 12/16 align: 4/8
    deserializer.ReadObject("minVel", m_minVel);                // offset: 92/128 size: 12/16 align: 4/8
    deserializer.ReadObject("maxVel", m_maxVel);                // offset: 104/144 size: 12/16 align: 4/8
    deserializer.ReadObject("allBonesDown", m_allBonesDown);    // offset: 116/160 size: 12/16 align: 4/8
    deserializer.ReadObject("anyBonesDown", m_anyBonesDown);    // offset: 128/176 size: 12/16 align: 4/8
    deserializer.ReadValue("posTol", m_posTol);                 // offset: 140/192 size: 4/4 align: 4/4
    deserializer.ReadValue("velTol", m_velTol);                 // offset: 144/196 size: 4/4 align: 4/4
    deserializer.ReadValue("duration", m_duration);             // offset: 148/200 size: 4/4 align: 4/4
    deserializer.Pad(deserializer.GetPointerSize());            // offset: 152/204 size: 0/4
    // class size: 152/208 align: 4/8
}
