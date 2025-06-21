#include "Havok/hkpMoppCodeReindexedTerminal.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpMoppCodeReindexedTerminal,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpMoppCodeReindexedTerminal::Class; });

nemesis::hkpMoppCodeReindexedTerminal::hkpMoppCodeReindexedTerminal() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkpMoppCodeReindexedTerminal::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpMoppCodeReindexedTerminal::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("origShapeKey", m_origShapeKey);           // offset: 0/0 size: 4/4 align: 4/4
    serializer.WriteValue("reindexedShapeKey", m_reindexedShapeKey); // offset: 4/4 size: 4/4 align: 4/4
    // class size: 8/8 align: 4/4
}

void nemesis::hkpMoppCodeReindexedTerminal::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("origShapeKey", m_origShapeKey);           // offset: 0/0 size: 4/4 align: 4/4
    deserializer.ReadValue("reindexedShapeKey", m_reindexedShapeKey); // offset: 4/4 size: 4/4 align: 4/4
    // class size: 8/8 align: 4/4
}
