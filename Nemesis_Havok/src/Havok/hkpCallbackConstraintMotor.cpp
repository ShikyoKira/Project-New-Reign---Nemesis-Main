#include "Havok/hkpCallbackConstraintMotor.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpCallbackConstraintMotor,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpCallbackConstraintMotor::Class; });

nemesis::hkpCallbackConstraintMotor::hkpCallbackConstraintMotor() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkpLimitedForceConstraintMotor(Class)
{
}

const nemesis::hkClass* nemesis::hkpCallbackConstraintMotor::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpCallbackConstraintMotor::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpLimitedForceConstraintMotor::SerializeTo(serializer); // offset: 0/0 size: 20/32 align: 4/8
    serializer.WriteSerializeIgnoredObject("callbackFunc",
                                           m_callbackFunc); // offset: 20/32 size: 4/8 align: 4/8
    serializer.WriteValue("callbackType", m_callbackType);  // offset: 24/40 size: 4/4 align: 4/4
    serializer.Skip(4);                                     // offset: 28/44 size: 4/4

    serializer.WriteValue("userData0", m_userData0); // offset: 32/48 size: 8/8 align: 8/8
    serializer.WriteValue("userData1", m_userData1); // offset: 40/56 size: 8/8 align: 8/8
    serializer.WriteValue("userData2", m_userData2); // offset: 48/64 size: 8/8 align: 8/8
    // class size: 56/72 align: 8/8
}

void nemesis::hkpCallbackConstraintMotor::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpLimitedForceConstraintMotor::DeserializeFrom(
        deserializer);                                       // offset: 0/0 size: 20/32 align: 4/8
    deserializer.ReadObject("callbackFunc", m_callbackFunc); // offset: 20/32 size: 4/8 align: 4/8
    deserializer.ReadValue("callbackType", m_callbackType);  // offset: 24/40 size: 4/4 align: 4/4
    deserializer.Skip(4);                                    // offset: 28/44 size: 4/4

    deserializer.ReadValue("userData0", m_userData0); // offset: 32/48 size: 8/8 align: 8/8
    deserializer.ReadValue("userData1", m_userData1); // offset: 40/56 size: 8/8 align: 8/8
    deserializer.ReadValue("userData2", m_userData2); // offset: 48/64 size: 8/8 align: 8/8
    // class size: 56/72 align: 8/8
}
