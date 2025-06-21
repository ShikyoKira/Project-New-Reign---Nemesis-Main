#include "Havok/hkpLinMotorConstraintAtom.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpLinMotorConstraintAtom,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpLinMotorConstraintAtom::Class; });

nemesis::hkpLinMotorConstraintAtom::hkpLinMotorConstraintAtom() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkpConstraintAtom(Class)
{
}

const nemesis::hkClass* nemesis::hkpLinMotorConstraintAtom::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpLinMotorConstraintAtom::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpConstraintAtom::SerializeTo(serializer); // offset: 0/0 size: 2/2 align: 2/2
    serializer.WriteValue("isEnabled", m_isEnabled);     // offset: 2/2 size: 1/1 align: 1/1
    serializer.WriteValue("motorAxis", m_motorAxis);     // offset: 3/3 size: 1/1 align: 1/1

    if (serializer.GetContentsVersion() > HavokVersion::HK_2011_3_0)
    {
        serializer.WriteSerializeIgnoredValue("initializedOffset", m_initializedOffset);
        serializer.WriteSerializeIgnoredValue("previousTargetPositionOffset", m_previousTargetPositionOffset);
    }
    else
    {
        serializer.WriteValue("initializedOffset", m_initializedOffset); // offset: 4/4 size: 2/2 align: 2/2
        serializer.WriteValue("previousTargetPositionOffset",
                              m_previousTargetPositionOffset); // offset: 6/6 size: 2/2 align: 2/2
    }

    serializer.WriteValue("targetPosition", m_targetPosition); // offset: 8/8 size: 4/4 align: 4/4
    serializer.Pad(serializer.GetPointerSize());               // offset: 12/12 size: 0/4

    serializer.WriteObject("motor", m_motor); // offset: 12/16 size: 4/8 align: 4/8
    // class size: 16/24 align: 4/8
}

void nemesis::hkpLinMotorConstraintAtom::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpConstraintAtom::DeserializeFrom(deserializer);        // offset: 0/0 size: 2/2 align: 2/2
    deserializer.ReadValue("isEnabled", m_isEnabled);                 // offset: 2/2 size: 1/1 align: 1/1
    deserializer.ReadValue("motorAxis", m_motorAxis);                 // offset: 3/3 size: 1/1 align: 1/1
    deserializer.ReadValue("initializedOffset", m_initializedOffset); // offset: 4/4 size: 2/2 align: 2/2
    deserializer.ReadValue("previousTargetPositionOffset",
                           m_previousTargetPositionOffset);     // offset: 6/6 size: 2/2 align: 2/2
    deserializer.ReadValue("targetPosition", m_targetPosition); // offset: 8/8 size: 4/4 align: 4/4
    deserializer.Pad(deserializer.GetPointerSize());            // offset: 12/12 size: 0/4

    deserializer.ReadObject("motor", m_motor); // offset: 12/16 size: 4/8 align: 4/8
    // class size: 16/24 align: 4/8
}
