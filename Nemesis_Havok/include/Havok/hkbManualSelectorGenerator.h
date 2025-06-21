#pragma once

#include "Havok/Base/hkbGenerator.h"

#include "Havok/hkbCustomIdSelector.h"
#include "Havok/hkbStateMachineActiveTransitionInfo.h"

namespace nemesis
{
    struct hkbManualSelectorGenerator : nemesis::hkbGenerator
    {
        static constexpr nemesis::hkClass Class{0xd932fab8,
                                                "hkbManualSelectorGenerator",
                                                &nemesis::hkbGenerator::Class,
                                                96,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkbManualSelectorGenerator);

        nemesis::hkArray<nemesis::hkRefPtr<nemesis::hkbGenerator>> m_generators;
        char m_selectedGeneratorIndex;
        nemesis::hkbCustomIdSelector m_indexSelector;
        bool m_selectedIndexCanChangeAfterActivate;
        nemesis::hkRefPtr<nemesis::hkbTransitionEffect> m_generatorChangedTransitionEffect;
        char m_currentGeneratorIndex;
        char m_generatorIndexAtActivate;
        nemesis::hkArray<nemesis::hkbStateMachineActiveTransitionInfo> m_activeTransitions;

    public:
        hkbManualSelectorGenerator() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbManualSelectorGenerator::Class;
}
