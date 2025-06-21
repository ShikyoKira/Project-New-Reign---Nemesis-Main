#include "Havok/hkbEvaluateExpressionModifierInternalExpressionData.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbEvaluateExpressionModifierInternalExpressionData,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbEvaluateExpressionModifierInternalExpressionData::Class; });

nemesis::hkbEvaluateExpressionModifierInternalExpressionData::hkbEvaluateExpressionModifierInternalExpressionData() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkbEvaluateExpressionModifierInternalExpressionData::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbEvaluateExpressionModifierInternalExpressionData::SerializeTo(
    nemesis::Serializer& serializer) const
{
    serializer.WriteValue("raisedEvent", m_raisedEvent); // offset: 0/0 size: 1/1 align: 1/1
    serializer.WriteValue("wasTrueInPreviousFrame",
                          m_wasTrueInPreviousFrame); // offset: 1/1 size: 1/1 align: 1/1
    // class size: 2/2 align: 1/1
}

void nemesis::hkbEvaluateExpressionModifierInternalExpressionData::DeserializeFrom(
    nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("raisedEvent", m_raisedEvent); // offset: 0/0 size: 1/1 align: 1/1
    deserializer.ReadValue("wasTrueInPreviousFrame",
                           m_wasTrueInPreviousFrame); // offset: 1/1 size: 1/1 align: 1/1
    // class size: 2/2 align: 1/1
}
