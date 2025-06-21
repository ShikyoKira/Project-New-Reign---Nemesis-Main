#pragma once

#include "Havok/Base/hkbGenerator.h"

namespace nemesis
{
    struct hkbTransitionEffect : nemesis::hkbGenerator
    {
        static constexpr nemesis::hkClass Class{0x945da157,
                                                "hkbTransitionEffect",
                                                &nemesis::hkbGenerator::Class,
                                                80,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkbTransitionEffect);

        using SelfTransitionMode = nemesis::hkInternalClassEnumItem<char>;
        using EventMode = nemesis::hkInternalClassEnumItem<char>;

        nemesis::hkEnum<SelfTransitionMode, char> m_selfTransitionMode = {
            {"SELF_TRANSITION_MODE_CONTINUE_IF_CYCLIC_BLEND_IF_ACYCLIC", 0},
            {"SELF_TRANSITION_MODE_CONTINUE", 1},
            {"SELF_TRANSITION_MODE_RESET", 2},
            {"SELF_TRANSITION_MODE_BLEND", 3},
        };
        nemesis::hkEnum<EventMode, char> m_eventMode = {
            {"EVENT_MODE_DEFAULT", 0},
            {"EVENT_MODE_PROCESS_ALL", 1},
            {"EVENT_MODE_IGNORE_FROM_GENERATOR", 2},
            {"EVENT_MODE_IGNORE_TO_GENERATOR", 3},
        };
        char m_defaultEventMode;

    public:
        hkbTransitionEffect() noexcept;
        hkbTransitionEffect(const nemesis::hkClass& cls) noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbTransitionEffect::Class;
}
