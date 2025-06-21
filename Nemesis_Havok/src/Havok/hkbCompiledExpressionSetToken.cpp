#include "Havok/hkbCompiledExpressionSetToken.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbCompiledExpressionSetToken,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbCompiledExpressionSetToken::Class; });

nemesis::hkbCompiledExpressionSetToken::hkbCompiledExpressionSetToken() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkbCompiledExpressionSetToken::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbCompiledExpressionSetToken::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("data", m_data);         // offset: 0/0 size: 4/4 align: 4/4
    serializer.WriteValue("type", m_type);         // offset: 4/4 size: 1/1 align: 1/1
    serializer.WriteValue("operator", m_operator); // offset: 5/5 size: 1/1 align: 1/1
    serializer.Skip(2);                            // offset: 6/6 size: 2/2
    // class size: 8/8 align: 4/4
}

void nemesis::hkbCompiledExpressionSetToken::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("data", m_data);         // offset: 0/0 size: 4/4 align: 4/4
    deserializer.ReadValue("type", m_type);         // offset: 4/4 size: 1/1 align: 1/1
    deserializer.ReadValue("operator", m_operator); // offset: 5/5 size: 1/1 align: 1/1
    deserializer.Skip(2);                           // offset: 6/6 size: 2/2
    // class size: 8/8 align: 4/4
}
