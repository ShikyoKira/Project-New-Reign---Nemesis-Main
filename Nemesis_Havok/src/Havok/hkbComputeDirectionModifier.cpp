#include "Havok/hkbComputeDirectionModifier.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbComputeDirectionModifier,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbComputeDirectionModifier::Class; });

nemesis::hkbComputeDirectionModifier::hkbComputeDirectionModifier() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkbModifier(Class)
{
}

const nemesis::hkClass* nemesis::hkbComputeDirectionModifier::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbComputeDirectionModifier::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkbModifier::SerializeTo(serializer); // offset: 0/0 size: 44/80 align: 4/8
    serializer.Pad(16);                            // offset: 44/80 size: 4/0

    serializer.WriteValue("pointIn", m_pointIn);               // offset: 48/80 size: 16/16 align: 16/16
    serializer.WriteValue("pointOut", m_pointOut);             // offset: 64/96 size: 16/16 align: 16/16
    serializer.WriteValue("groundAngleOut", m_groundAngleOut); // offset: 80/112 size: 4/4 align: 4/4
    serializer.WriteValue("upAngleOut", m_upAngleOut);         // offset: 84/116 size: 4/4 align: 4/4
    serializer.WriteValue("verticalOffset", m_verticalOffset); // offset: 88/120 size: 4/4 align: 4/4
    serializer.WriteValue("reverseGroundAngle", m_reverseGroundAngle); // offset: 92/124 size: 1/1 align: 1/1
    serializer.WriteValue("reverseUpAngle", m_reverseUpAngle);         // offset: 93/125 size: 1/1 align: 1/1
    serializer.WriteValue("projectPoint", m_projectPoint);             // offset: 94/126 size: 1/1 align: 1/1
    serializer.WriteValue("normalizePoint", m_normalizePoint);         // offset: 95/127 size: 1/1 align: 1/1
    serializer.WriteValue("computeOnlyOnce", m_computeOnlyOnce);       // offset: 96/128 size: 1/1 align: 1/1
    serializer.WriteValue("computedOutput", m_computedOutput);         // offset: 97/129 size: 1/1 align: 1/1
    serializer.Skip(14);                                               // offset: 98/130 size: 14/14
    // class size: 112/144 align: 16/16
}

void nemesis::hkbComputeDirectionModifier::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkbModifier::DeserializeFrom(deserializer); // offset: 0/0 size: 44/80 align: 4/8
    deserializer.Pad(16);                                // offset: 44/80 size: 4/0

    deserializer.ReadValue("pointIn", m_pointIn);               // offset: 48/80 size: 16/16 align: 16/16
    deserializer.ReadValue("pointOut", m_pointOut);             // offset: 64/96 size: 16/16 align: 16/16
    deserializer.ReadValue("groundAngleOut", m_groundAngleOut); // offset: 80/112 size: 4/4 align: 4/4
    deserializer.ReadValue("upAngleOut", m_upAngleOut);         // offset: 84/116 size: 4/4 align: 4/4
    deserializer.ReadValue("verticalOffset", m_verticalOffset); // offset: 88/120 size: 4/4 align: 4/4
    deserializer.ReadValue("reverseGroundAngle", m_reverseGroundAngle); // offset: 92/124 size: 1/1 align: 1/1
    deserializer.ReadValue("reverseUpAngle", m_reverseUpAngle);         // offset: 93/125 size: 1/1 align: 1/1
    deserializer.ReadValue("projectPoint", m_projectPoint);             // offset: 94/126 size: 1/1 align: 1/1
    deserializer.ReadValue("normalizePoint", m_normalizePoint);         // offset: 95/127 size: 1/1 align: 1/1
    deserializer.ReadValue("computeOnlyOnce", m_computeOnlyOnce);       // offset: 96/128 size: 1/1 align: 1/1
    deserializer.ReadValue("computedOutput", m_computedOutput);         // offset: 97/129 size: 1/1 align: 1/1
    deserializer.Skip(14);                                              // offset: 98/130 size: 14/14
    // class size: 112/144 align: 16/16
}
