#include "Havok/hkbBehaviorGraphData.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbBehaviorGraphData,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbBehaviorGraphData::Class; });

nemesis::hkbBehaviorGraphData::hkbBehaviorGraphData() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkbBehaviorGraphStringData* nemesis::hkbBehaviorGraphData::GetStringData() const
{
    return m_stringData.IsNull() ? nullptr : &m_stringData.GetReferencedObject();
}

const nemesis::hkClass* nemesis::hkbBehaviorGraphData::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbBehaviorGraphData::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkReferencedObject::SerializeTo(serializer);             // offset: 0/0 size: 8/16 align: 4/8
    serializer.WriteObject("attributeDefaults", m_attributeDefaults); // offset: 8/16 size: 12/16 align: 4/8
    serializer.WriteObject("variableInfos", m_variableInfos);         // offset: 20/32 size: 12/16 align: 4/8
    serializer.WriteObject("characterPropertyInfos",
                           m_characterPropertyInfos);   // offset: 32/48 size: 12/16 align: 4/8
    serializer.WriteObject("eventInfos", m_eventInfos); // offset: 44/64 size: 12/16 align: 4/8

    if (serializer.GetContentsVersion() > HavokVersion::HK_2010_2_0)
    {
        serializer.WriteObject("variableBounds", m_variableBounds);
    }
    else
    {
        serializer.WriteObject("wordMinVariableValues",
                               m_wordMinVariableValues); // offset: 56/80 size: 12/16 align: 4/8
        serializer.WriteObject("wordMaxVariableValues",
                               m_wordMaxVariableValues); // offset: 68/96 size: 12/16 align: 4/8
    }

    serializer.WriteObject("variableInitialValues",
                           m_variableInitialValues);    // offset: 80/112 size: 4/8 align: 4/8
    serializer.WriteObject("stringData", m_stringData); // offset: 84/120 size: 4/8 align: 4/8
    // class size: 88/128 align: 4/8
}

void nemesis::hkbBehaviorGraphData::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkReferencedObject::DeserializeFrom(deserializer);        // offset: 0/0 size: 8/16 align: 4/8
    deserializer.ReadObject("attributeDefaults", m_attributeDefaults); // offset: 8/16 size: 12/16 align: 4/8
    deserializer.ReadObject("variableInfos", m_variableInfos);         // offset: 20/32 size: 12/16 align: 4/8
    deserializer.ReadObject("characterPropertyInfos",
                            m_characterPropertyInfos);   // offset: 32/48 size: 12/16 align: 4/8
    deserializer.ReadObject("eventInfos", m_eventInfos); // offset: 44/64 size: 12/16 align: 4/8

    if (deserializer.GetContentsVersion() > HavokVersion::HK_2010_2_0)
    {
        deserializer.ReadObject("variableBounds", m_variableBounds);
    }
    else
    {
        deserializer.ReadObject("wordMinVariableValues",
                                m_wordMinVariableValues); // offset: 56/80 size: 12/16 align: 4/8
        deserializer.ReadObject("wordMaxVariableValues",
                                m_wordMaxVariableValues); // offset: 68/96 size: 12/16 align: 4/8
    }

    deserializer.ReadObject("variableInitialValues",
                            m_variableInitialValues);    // offset: 80/112 size: 4/8 align: 4/8
    deserializer.ReadObject("stringData", m_stringData); // offset: 84/120 size: 4/8 align: 4/8
    // class size: 88/128 align: 4/8
}
