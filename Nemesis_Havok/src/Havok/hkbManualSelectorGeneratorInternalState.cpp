#include "Havok/hkbManualSelectorGeneratorInternalState.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbManualSelectorGeneratorInternalState,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbManualSelectorGeneratorInternalState::Class; });

nemesis::hkbManualSelectorGeneratorInternalState::hkbManualSelectorGeneratorInternalState() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkbManualSelectorGeneratorInternalState::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbManualSelectorGeneratorInternalState::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkReferencedObject::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8
    serializer.WriteValue("currentGeneratorIndex",
                          m_currentGeneratorIndex); // offset: 8/16 size: 1/1 align: 1/1

    if (serializer.GetContentsVersion() > HavokVersion::HK_2011_1_0)
    {
        serializer.WriteValue("generatorIndexAtActivate", m_generatorIndexAtActivate);
        serializer.Pad(serializer.GetPointerSize());

        serializer.WriteObject("activeTransitions", m_activeTransitions);
    }

    serializer.Pad(serializer.GetPointerSize()); // offset: 9/17 size: 3/7
    // class size: 12/24 align: 4/8
}

void nemesis::hkbManualSelectorGeneratorInternalState::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    deserializer.ReadValue("currentGeneratorIndex",
                           m_currentGeneratorIndex); // offset: 8/16 size: 1/1 align: 1/1

    if (deserializer.GetContentsVersion() > HavokVersion::HK_2011_1_0)
    {
        deserializer.ReadValue("generatorIndexAtActivate", m_generatorIndexAtActivate);
        deserializer.Pad(deserializer.GetPointerSize());

        deserializer.ReadObject("activeTransitions", m_activeTransitions);
    }

    deserializer.Pad(deserializer.GetPointerSize()); // offset: 9/17 size: 3/7
    // class size: 12/24 align: 4/8
}
