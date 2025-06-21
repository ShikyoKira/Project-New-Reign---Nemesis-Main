#include "Havok/hkbExpressionCondition.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbExpressionCondition,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbExpressionCondition::Class; });

nemesis::hkbExpressionCondition::hkbExpressionCondition() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkbCondition(Class)
{
}

const nemesis::hkClass* nemesis::hkbExpressionCondition::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbExpressionCondition::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkbCondition::SerializeTo(serializer);    // offset: 0/0 size: 8/16 align: 4/8
    serializer.WriteValue("expression", m_expression); // offset: 8/16 size: 4/8 align: 4/8

    if (serializer.GetContentsVersion() > HavokVersion::HK_2010_1_0)
    {
        serializer.WriteSerializeIgnoredObject("compiledExpressionSet",
                                               m_compiledExpressionSet); // offset: 12/24 size: 4/8 align: 4/8
    }
    else
    {
        serializer.WriteObject("compiledExpressionSet", m_compiledExpressionSet);
    }

    // class size: 16/32 align: 4/8
}

void nemesis::hkbExpressionCondition::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkbCondition::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    deserializer.ReadValue("expression", m_expression);   // offset: 8/16 size: 4/8 align: 4/8
    deserializer.ReadObject("compiledExpressionSet",
                            m_compiledExpressionSet); // offset: 12/24 size: 4/8 align: 4/8
    // class size: 16/32 align: 4/8
}
