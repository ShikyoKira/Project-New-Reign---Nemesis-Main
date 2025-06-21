#pragma once

#include "Havok/Base/hkReferencedObject.h"

namespace nemesis
{
    struct hkbSimulationStateInfo : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0xa40822b4,
                                                "hkbSimulationStateInfo",
                                                &nemesis::hkReferencedObject::Class,
                                                24,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkbSimulationStateInfo);

        using SimulationState = nemesis::hkInternalClassEnumItem<unsigned char>;

        nemesis::hkEnum<SimulationState, unsigned char> m_simulationState = {
            {"SIMULATION_STATE_PLAY", 0},
            {"SIMULATION_STATE_PAUSE", 1},
            {"SIMULATION_STATE_STEP", 2},
            {"SIMULATION_STATE_STOP", 3},
        };

    public:
        hkbSimulationStateInfo() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbSimulationStateInfo::Class;
}
