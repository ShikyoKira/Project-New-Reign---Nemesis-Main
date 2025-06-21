#include "Havok/hkpEntityExtendedListeners.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpEntityExtendedListeners,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpEntityExtendedListeners::Class; });

nemesis::hkpEntityExtendedListeners::hkpEntityExtendedListeners() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

nemesis::hkpEntityExtendedListeners::hkpEntityExtendedListeners(const nemesis::hkClass& cls) noexcept
    : nemesis::HavokObject(cls.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkpEntityExtendedListeners::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpEntityExtendedListeners::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteSerializeIgnoredObject("activationListeners",
                                           m_activationListeners); // offset: 0/0 size: 8/16 align: 4/8
    serializer.WriteSerializeIgnoredObject("entityListeners",
                                           m_entityListeners); // offset: 8/16 size: 8/16 align: 4/8
    // class size: 16/32 align: 4/8
}

void nemesis::hkpEntityExtendedListeners::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadObject("activationListeners",
                            m_activationListeners);                // offset: 0/0 size: 8/16 align: 4/8
    deserializer.ReadObject("entityListeners", m_entityListeners); // offset: 8/16 size: 8/16 align: 4/8
    // class size: 16/32 align: 4/8
}
