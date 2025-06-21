#include "Havok/hkbStateMachineStateInfo.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbStateMachineStateInfo,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbStateMachineStateInfo::Class; });

nemesis::hkbStateMachineStateInfo::hkbStateMachineStateInfo() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkbBindable(Class)
{
}

const nemesis::hkClass* nemesis::hkbStateMachineStateInfo::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbStateMachineStateInfo::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkbBindable::SerializeTo(serializer);    // offset: 0/0 size: 28/48 align: 4/8
    serializer.WriteObject("listeners", m_listeners); // offset: 28/48 size: 12/16 align: 4/8

    if (serializer.GetContentsVersion() > HavokVersion::HK_2010_1_0)
    {
        serializer.WriteObject("enterNotifyEvents",
                               m_enterNotifyEvents);                    // offset: 40/64 size: 4/8 align: 4/8
        serializer.WriteObject("exitNotifyEvents", m_exitNotifyEvents); // offset: 44/72 size: 4/8 align: 4/8
    }
    else
    {
        serializer.WriteObject("enterNotifyEvent", m_enterNotifyEvent);
        serializer.WriteObject("exitNotifyEvent", m_exitNotifyEvent);
    }

    serializer.WriteObject("transitions", m_transitions); // offset: 48/80 size: 4/8 align: 4/8
    serializer.WriteObject("generator", m_generator);     // offset: 52/88 size: 4/8 align: 4/8
    serializer.WriteValue("name", m_name);                // offset: 56/96 size: 4/8 align: 4/8
    serializer.WriteValue("stateId", m_stateId);          // offset: 60/104 size: 4/4 align: 4/4
    serializer.WriteValue("probability", m_probability);  // offset: 64/108 size: 4/4 align: 4/4
    serializer.WriteValue("enable", m_enable);            // offset: 68/112 size: 1/1 align: 1/1
    serializer.Pad(serializer.GetPointerSize());          // offset: 69/113 size: 3/7
    // class size: 72/120 align: 4/8
}

void nemesis::hkbStateMachineStateInfo::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkbBindable::DeserializeFrom(deserializer); // offset: 0/0 size: 28/48 align: 4/8
    deserializer.ReadObject("listeners", m_listeners);   // offset: 28/48 size: 12/16 align: 4/8

    if (deserializer.GetContentsVersion() > HavokVersion::HK_2010_1_0)
    {
        deserializer.ReadObject("enterNotifyEvents",
                                m_enterNotifyEvents);                    // offset: 40/64 size: 4/8 align: 4/8
        deserializer.ReadObject("exitNotifyEvents", m_exitNotifyEvents); // offset: 44/72 size: 4/8 align: 4/8
    }
    else
    {
        deserializer.ReadObject("enterNotifyEvent", m_enterNotifyEvent);
        deserializer.ReadObject("exitNotifyEvent", m_exitNotifyEvent);
    }

    deserializer.ReadObject("transitions", m_transitions); // offset: 48/80 size: 4/8 align: 4/8
    deserializer.ReadObject("generator", m_generator);     // offset: 52/88 size: 4/8 align: 4/8
    deserializer.ReadValue("name", m_name);                // offset: 56/96 size: 4/8 align: 4/8
    deserializer.ReadValue("stateId", m_stateId);          // offset: 60/104 size: 4/4 align: 4/4
    deserializer.ReadValue("probability", m_probability);  // offset: 64/108 size: 4/4 align: 4/4
    deserializer.ReadValue("enable", m_enable);            // offset: 68/112 size: 1/1 align: 1/1
    deserializer.Pad(deserializer.GetPointerSize());       // offset: 69/113 size: 3/7
    // class size: 72/120 align: 4/8
}
