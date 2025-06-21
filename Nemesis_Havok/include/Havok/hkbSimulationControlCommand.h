#pragma once

#include "Havok/Base/hkReferencedObject.h"

namespace nemesis
{
    struct hkbSimulationControlCommand : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0x2a241367,
                                                "hkbSimulationControlCommand",
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
        REGISTER_HAVOK_POINTER_HEADER(hkbSimulationControlCommand);

        using SimulationControlCommand = nemesis::hkInternalClassEnumItem<unsigned char>;

        nemesis::hkEnum<SimulationControlCommand, unsigned char> m_command = {
            {"COMMAND_PLAY", 0},
            {"COMMAND_PAUSE", 1},
            {"COMMAND_STEP", 2},
            {"COMMAND_STOP", 3},
            {"COMMAND_ACCUMULATE_MOTION", 4},
            {"COMMAND_DO_NOT_ACCUMULATE_MOTION", 5},
        };

    public:
        hkbSimulationControlCommand() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbSimulationControlCommand::Class;
}
