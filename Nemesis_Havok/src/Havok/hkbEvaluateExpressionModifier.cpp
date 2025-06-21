#include "Havok/hkbEvaluateExpressionModifier.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbEvaluateExpressionModifier,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbEvaluateExpressionModifier::Class; });

nemesis::hkbEvaluateExpressionModifier::hkbEvaluateExpressionModifier() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkbModifier(Class)
{
}

const nemesis::hkClass* nemesis::hkbEvaluateExpressionModifier::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbEvaluateExpressionModifier::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkbModifier::SerializeTo(serializer);        // offset: 0/0 size: 44/80 align: 4/8
    serializer.WriteObject("expressions", m_expressions); // offset: 44/80 size: 4/8 align: 4/8

    if (serializer.GetContentsVersion() > HavokVersion::HK_2010_1_0)
    {
        serializer.WriteSerializeIgnoredObject("compiledExpressionSet",
                                               m_compiledExpressionSet); // offset: 48/88 size: 4/8 align: 4/8
    }
    else
    {
        serializer.WriteObject("compiledExpressionSet",
                               m_compiledExpressionSet); // offset: 48/88 size: 4/8 align: 4/8
    }

    serializer.WriteSerializeIgnoredObject("internalExpressionsData",
                                           m_internalExpressionsData); // offset: 52/96 size: 12/16 align: 4/8
    // class size: 64/112 align: 4/8
}

void nemesis::hkbEvaluateExpressionModifier::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkbModifier::DeserializeFrom(deserializer);   // offset: 0/0 size: 44/80 align: 4/8
    deserializer.ReadObject("expressions", m_expressions); // offset: 44/80 size: 4/8 align: 4/8
    deserializer.ReadObject("compiledExpressionSet",
                            m_compiledExpressionSet); // offset: 48/88 size: 4/8 align: 4/8
    deserializer.ReadObject("internalExpressionsData",
                            m_internalExpressionsData); // offset: 52/96 size: 12/16 align: 4/8
    // class size: 64/112 align: 4/8
}
