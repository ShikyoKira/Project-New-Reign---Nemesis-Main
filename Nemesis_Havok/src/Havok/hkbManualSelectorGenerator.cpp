#include "Havok/hkbManualSelectorGenerator.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbManualSelectorGenerator,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbManualSelectorGenerator::Class; });

nemesis::hkbManualSelectorGenerator::hkbManualSelectorGenerator() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkbGenerator(Class)
{
}

const nemesis::hkClass* nemesis::hkbManualSelectorGenerator::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbManualSelectorGenerator::SerializeTo(nemesis::Serializer& serializer) const
{
    unsigned int padding_size = serializer.GetPointerSize();

    nemesis::hkbGenerator::SerializeTo(serializer); // offset: 0/0 size: 40/72 align: 4/8
    serializer.Pad(padding_size);

    serializer.WriteObject("generators", m_generators); // offset: 40/72 size: 12/16 align: 4/8
    serializer.WriteValue("selectedGeneratorIndex",
                          m_selectedGeneratorIndex); // offset: 52/88 size: 1/1 align: 1/1

    auto cur_ver = serializer.GetContentsVersion();

    if (cur_ver > HavokVersion::HK_2013_2_0)
    {
        serializer.Pad(padding_size);

        serializer.WriteObject("indexSelector", m_indexSelector);
        serializer.WriteValue("selectedIndexCanChangeAfterActivate", m_selectedIndexCanChangeAfterActivate);
        serializer.Pad(padding_size);

        serializer.WriteObject("generatorChangedTransitionEffect", m_generatorChangedTransitionEffect);
        serializer.WriteSerializeIgnoredValue("currentGeneratorIndex", m_currentGeneratorIndex);
        serializer.WriteSerializeIgnoredValue("generatorIndexAtActivate", m_generatorIndexAtActivate);
        serializer.Pad(padding_size);

        serializer.WriteSerializeIgnoredObject("activeTransitions", m_activeTransitions);
    }
    else if (cur_ver > HavokVersion::HK_2011_1_0)
    {
        serializer.WriteValue("selectedIndexCanChangeAfterActivate", m_selectedIndexCanChangeAfterActivate);
        serializer.Pad(padding_size);

        serializer.WriteObject("generatorChangedTransitionEffect", m_generatorChangedTransitionEffect);
        serializer.WriteValue("currentGeneratorIndex", m_currentGeneratorIndex);
        serializer.WriteValue("generatorIndexAtActivate", m_generatorIndexAtActivate);
        serializer.Pad(padding_size);

        serializer.WriteSerializeIgnoredObject("activeTransitions", m_activeTransitions);
    }
    else if (cur_ver > HavokVersion::HK_2010_2_0)
    {
        serializer.WriteValue("selectedIndexCanChangeAfterActivate", m_selectedIndexCanChangeAfterActivate);
        serializer.WriteValue("currentGeneratorIndex", m_currentGeneratorIndex);
        serializer.WriteValue("generatorIndexAtActivate", m_generatorIndexAtActivate);
    }
    else
    {
        serializer.WriteValue("currentGeneratorIndex",
                              m_currentGeneratorIndex); // offset: 53/89 size: 1/1 align: 1/1
    }

    serializer.Pad(padding_size); // offset: 54/90 size: 2/6
    // class size: 56/96 align: 4/8
}

void nemesis::hkbManualSelectorGenerator::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    unsigned int padding_size = deserializer.GetPointerSize();

    nemesis::hkbGenerator::DeserializeFrom(deserializer); // offset: 0/0 size: 40/72 align: 4/8
    deserializer.Pad(padding_size);

    deserializer.ReadObject("generators", m_generators);  // offset: 40/72 size: 12/16 align: 4/8
    deserializer.ReadValue("selectedGeneratorIndex",
                           m_selectedGeneratorIndex); // offset: 52/88 size: 1/1 align: 1/1

    auto cur_ver = deserializer.GetContentsVersion();

    if (cur_ver > HavokVersion::HK_2013_2_0)
    {
        deserializer.Pad(padding_size);

        deserializer.ReadObject("indexSelector", m_indexSelector);
        deserializer.ReadValue("selectedIndexCanChangeAfterActivate", m_selectedIndexCanChangeAfterActivate);
        deserializer.Pad(padding_size);

        deserializer.ReadObject("generatorChangedTransitionEffect", m_generatorChangedTransitionEffect);
        deserializer.ReadValue("currentGeneratorIndex", m_currentGeneratorIndex);
        deserializer.ReadValue("generatorIndexAtActivate", m_generatorIndexAtActivate);
        deserializer.Pad(padding_size);

        deserializer.ReadObject("activeTransitions", m_activeTransitions);
    }
    else if (cur_ver > HavokVersion::HK_2011_1_0)
    {
        deserializer.ReadValue("selectedIndexCanChangeAfterActivate", m_selectedIndexCanChangeAfterActivate);
        deserializer.Pad(padding_size);

        deserializer.ReadObject("generatorChangedTransitionEffect", m_generatorChangedTransitionEffect);
        deserializer.ReadValue("currentGeneratorIndex", m_currentGeneratorIndex);
        deserializer.ReadValue("generatorIndexAtActivate", m_generatorIndexAtActivate);
        deserializer.Pad(padding_size);

        deserializer.ReadObject("activeTransitions", m_activeTransitions);
    }
    else if (cur_ver > HavokVersion::HK_2010_2_0)
    {
        deserializer.ReadValue("selectedIndexCanChangeAfterActivate", m_selectedIndexCanChangeAfterActivate);
        deserializer.ReadValue("currentGeneratorIndex", m_currentGeneratorIndex);
        deserializer.ReadValue("generatorIndexAtActivate", m_generatorIndexAtActivate);
    }
    else
    {
        deserializer.ReadValue("currentGeneratorIndex",
                               m_currentGeneratorIndex); // offset: 53/89 size: 1/1 align: 1/1
    }

    deserializer.Pad(padding_size); // offset: 54/90 size: 2/6
    // class size: 56/96 align: 4/8
}
