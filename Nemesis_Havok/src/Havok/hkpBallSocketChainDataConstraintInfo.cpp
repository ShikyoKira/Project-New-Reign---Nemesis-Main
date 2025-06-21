#include "Havok/hkpBallSocketChainDataConstraintInfo.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpBallSocketChainDataConstraintInfo,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpBallSocketChainDataConstraintInfo::Class; });

nemesis::hkpBallSocketChainDataConstraintInfo::hkpBallSocketChainDataConstraintInfo() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkpBallSocketChainDataConstraintInfo::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpBallSocketChainDataConstraintInfo::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("pivotInA", m_pivotInA); // offset: 0/0 size: 16/16 align: 16/16
    serializer.WriteValue("pivotInB", m_pivotInB); // offset: 16/16 size: 16/16 align: 16/16
    // class size: 32/32 align: 16/16
}

void nemesis::hkpBallSocketChainDataConstraintInfo::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("pivotInA", m_pivotInA); // offset: 0/0 size: 16/16 align: 16/16
    deserializer.ReadValue("pivotInB", m_pivotInB); // offset: 16/16 size: 16/16 align: 16/16
    // class size: 32/32 align: 16/16
}
