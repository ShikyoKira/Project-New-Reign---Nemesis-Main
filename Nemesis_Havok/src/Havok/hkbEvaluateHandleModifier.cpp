#include "Havok/hkbEvaluateHandleModifier.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbEvaluateHandleModifier,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbEvaluateHandleModifier::Class; });

nemesis::hkbEvaluateHandleModifier::hkbEvaluateHandleModifier() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkbModifier(Class)
{
}

const nemesis::hkClass* nemesis::hkbEvaluateHandleModifier::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbEvaluateHandleModifier::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkbModifier::SerializeTo(serializer); // offset: 0/0 size: 44/80 align: 4/8
    serializer.WriteObject("handle", m_handle);    // offset: 44/80 size: 4/8 align: 4/8
    serializer.Pad(16);                            // offset: 48/88 size: 0/8

    serializer.WriteValue("handlePositionOut", m_handlePositionOut); // offset: 48/96 size: 16/16 align: 16/16
    serializer.WriteValue("handleRotationOut",
                          m_handleRotationOut);        // offset: 64/112 size: 16/16 align: 16/16
    serializer.WriteValue("isValidOut", m_isValidOut); // offset: 80/128 size: 1/1 align: 1/1
    serializer.Skip(3);                                // offset: 81/129 size: 3/3

    serializer.WriteValue("extrapolationTimeStep",
                          m_extrapolationTimeStep);                  // offset: 84/132 size: 4/4 align: 4/4
    serializer.WriteValue("handleChangeSpeed", m_handleChangeSpeed); // offset: 88/136 size: 4/4 align: 4/4
    serializer.WriteValue("handleChangeMode", m_handleChangeMode);   // offset: 92/140 size: 1/1 align: 1/1
    serializer.Skip(3);                                              // offset: 93/141 size: 3/3

    serializer.WriteSerializeIgnoredObject("oldHandle", m_oldHandle); // offset: 96/144 size: 24/48 align: 4/8
    serializer.Pad(16);                                               // offset: 120/192 size: 8/0

    serializer.WriteSerializeIgnoredValue("oldHandlePosition",
                                          m_oldHandlePosition); // offset: 128/192 size: 16/16 align: 16/16
    serializer.WriteSerializeIgnoredValue("oldHandleRotation",
                                          m_oldHandleRotation); // offset: 144/208 size: 16/16 align: 16/16
    serializer.WriteSerializeIgnoredValue("timeSinceLastModify",
                                          m_timeSinceLastModify); // offset: 160/224 size: 4/4 align: 4/4
    serializer.WriteSerializeIgnoredValue("smoothlyChangingHandles",
                                          m_smoothlyChangingHandles); // offset: 164/228 size: 1/1 align: 1/1
    serializer.Skip(11);                                              // offset: 165/229 size: 11/11
    // class size: 176/240 align: 16/16
}

void nemesis::hkbEvaluateHandleModifier::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkbModifier::DeserializeFrom(deserializer); // offset: 0/0 size: 44/80 align: 4/8
    deserializer.ReadObject("handle", m_handle);         // offset: 44/80 size: 4/8 align: 4/8
    deserializer.Pad(16);                                // offset: 48/88 size: 0/8

    deserializer.ReadValue("handlePositionOut",
                           m_handlePositionOut); // offset: 48/96 size: 16/16 align: 16/16
    deserializer.ReadValue("handleRotationOut",
                           m_handleRotationOut);        // offset: 64/112 size: 16/16 align: 16/16
    deserializer.ReadValue("isValidOut", m_isValidOut); // offset: 80/128 size: 1/1 align: 1/1
    deserializer.Skip(3);                               // offset: 81/129 size: 3/3

    deserializer.ReadValue("extrapolationTimeStep",
                           m_extrapolationTimeStep);                  // offset: 84/132 size: 4/4 align: 4/4
    deserializer.ReadValue("handleChangeSpeed", m_handleChangeSpeed); // offset: 88/136 size: 4/4 align: 4/4
    deserializer.ReadValue("handleChangeMode", m_handleChangeMode);   // offset: 92/140 size: 1/1 align: 1/1
    deserializer.Skip(3);                                             // offset: 93/141 size: 3/3

    deserializer.ReadObject("oldHandle", m_oldHandle); // offset: 96/144 size: 24/48 align: 4/8
    deserializer.Pad(16);                              // offset: 120/192 size: 8/0

    deserializer.ReadValue("oldHandlePosition",
                           m_oldHandlePosition); // offset: 128/192 size: 16/16 align: 16/16
    deserializer.ReadValue("oldHandleRotation",
                           m_oldHandleRotation); // offset: 144/208 size: 16/16 align: 16/16
    deserializer.ReadValue("timeSinceLastModify",
                           m_timeSinceLastModify); // offset: 160/224 size: 4/4 align: 4/4
    deserializer.ReadValue("smoothlyChangingHandles",
                           m_smoothlyChangingHandles); // offset: 164/228 size: 1/1 align: 1/1
    deserializer.Skip(11);                             // offset: 165/229 size: 11/11
    // class size: 176/240 align: 16/16
}
