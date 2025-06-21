#include "Havok/hkpSimulation.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpSimulation,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpSimulation::Class; });

nemesis::hkpSimulation::hkpSimulation() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkpSimulation::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpSimulation::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkReferencedObject::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8
    serializer.WriteValue("determinismCheckFrameCounter",
                          m_determinismCheckFrameCounter); // offset: 8/16 size: 4/4 align: 4/4
    serializer.Pad(serializer.GetPointerSize());           // offset: 12/20 size: 0/4

    serializer.WriteObject("world", m_world);                          // offset: 12/24 size: 4/8 align: 4/8
    serializer.WriteValue("lastProcessingStep", m_lastProcessingStep); // offset: 16/32 size: 1/1 align: 1/1
    serializer.Skip(3);                                                // offset: 17/33 size: 3/3

    serializer.WriteValue("currentTime", m_currentTime);               // offset: 20/36 size: 4/4 align: 4/4
    serializer.WriteValue("currentPsiTime", m_currentPsiTime);         // offset: 24/40 size: 4/4 align: 4/4
    serializer.WriteValue("physicsDeltaTime", m_physicsDeltaTime);     // offset: 28/44 size: 4/4 align: 4/4
    serializer.WriteValue("simulateUntilTime", m_simulateUntilTime);   // offset: 32/48 size: 4/4 align: 4/4
    serializer.WriteValue("frameMarkerPsiSnap", m_frameMarkerPsiSnap); // offset: 36/52 size: 4/4 align: 4/4
    serializer.WriteValue("previousStepResult", m_previousStepResult); // offset: 40/56 size: 4/4 align: 4/4
    serializer.Pad(serializer.GetPointerSize());                       // offset: 44/60 size: 0/4
    // class size: 44/64 align: 4/8
}

void nemesis::hkpSimulation::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    deserializer.ReadValue("determinismCheckFrameCounter",
                           m_determinismCheckFrameCounter); // offset: 8/16 size: 4/4 align: 4/4
    deserializer.Pad(deserializer.GetPointerSize());        // offset: 12/20 size: 0/4

    deserializer.ReadObject("world", m_world);                          // offset: 12/24 size: 4/8 align: 4/8
    deserializer.ReadValue("lastProcessingStep", m_lastProcessingStep); // offset: 16/32 size: 1/1 align: 1/1
    deserializer.Skip(3);                                               // offset: 17/33 size: 3/3

    deserializer.ReadValue("currentTime", m_currentTime);               // offset: 20/36 size: 4/4 align: 4/4
    deserializer.ReadValue("currentPsiTime", m_currentPsiTime);         // offset: 24/40 size: 4/4 align: 4/4
    deserializer.ReadValue("physicsDeltaTime", m_physicsDeltaTime);     // offset: 28/44 size: 4/4 align: 4/4
    deserializer.ReadValue("simulateUntilTime", m_simulateUntilTime);   // offset: 32/48 size: 4/4 align: 4/4
    deserializer.ReadValue("frameMarkerPsiSnap", m_frameMarkerPsiSnap); // offset: 36/52 size: 4/4 align: 4/4
    deserializer.ReadValue("previousStepResult", m_previousStepResult); // offset: 40/56 size: 4/4 align: 4/4
    deserializer.Pad(deserializer.GetPointerSize());                    // offset: 44/60 size: 0/4
    // class size: 44/64 align: 4/8
}
