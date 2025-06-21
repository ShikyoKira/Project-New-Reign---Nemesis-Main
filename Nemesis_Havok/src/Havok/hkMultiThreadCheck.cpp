#include "Havok/hkMultiThreadCheck.h"

REGISTER_HAVOK_POINTER_SOURCE(hkMultiThreadCheck,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkMultiThreadCheck::Class; });

nemesis::hkMultiThreadCheck::hkMultiThreadCheck() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkMultiThreadCheck::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkMultiThreadCheck::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteSerializeIgnoredValue("threadId", m_threadId);         // offset: 0/0 size: 4/4 align: 4/4
    serializer.WriteSerializeIgnoredValue("stackTraceId", m_stackTraceId); // offset: 4/4 size: 4/4 align: 4/4
    serializer.WriteSerializeIgnoredValue("markCount", m_markCount);       // offset: 8/8 size: 2/2 align: 2/2
    serializer.WriteSerializeIgnoredValue("markBitStack",
                                          m_markBitStack); // offset: 10/10 size: 2/2 align: 2/2
    // class size: 12/12 align: 4/4
}

void nemesis::hkMultiThreadCheck::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("threadId", m_threadId);         // offset: 0/0 size: 4/4 align: 4/4
    deserializer.ReadValue("stackTraceId", m_stackTraceId); // offset: 4/4 size: 4/4 align: 4/4
    deserializer.ReadValue("markCount", m_markCount);       // offset: 8/8 size: 2/2 align: 2/2
    deserializer.ReadValue("markBitStack", m_markBitStack); // offset: 10/10 size: 2/2 align: 2/2
    // class size: 12/12 align: 4/4
}
