#pragma once

#include "Havok/Base/hkReferencedObject.h"

namespace nemesis
{
    struct hkpWorld;

    struct hkpSimulation : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0x97aba922,
                                                "hkpSimulation",
                                                &nemesis::hkReferencedObject::Class,
                                                64,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpSimulation);

        using LastProcessingStep = nemesis::hkInternalClassEnumItem<unsigned char>;

        unsigned int m_determinismCheckFrameCounter{};
        nemesis::hkpWorld* m_world                                              = nullptr;
        nemesis::hkEnum<LastProcessingStep, unsigned char> m_lastProcessingStep = {
            {"INTEGRATE", 0},
            {"COLLIDE", 1},
        };
        float m_currentTime{};
        float m_currentPsiTime{};
        float m_physicsDeltaTime{};
        float m_simulateUntilTime{};
        float m_frameMarkerPsiSnap{};
        unsigned int m_previousStepResult{};

    public:
        hkpSimulation() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpSimulation::Class;
}
