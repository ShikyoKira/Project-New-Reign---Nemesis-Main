#pragma once

#include "Havok/hkStepInfo.h"
#include "Havok/hkpSolverInfo.h"

namespace nemesis
{
    struct hkpWorldDynamicsStepInfo : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0xb2b41feb,
                                                "hkpWorldDynamicsStepInfo",
                                                nullptr,
                                                320,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpWorldDynamicsStepInfo);

        nemesis::hkStepInfo m_stepInfo;
        nemesis::hkpSolverInfo m_solverInfo;

    public:
        hkpWorldDynamicsStepInfo() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpWorldDynamicsStepInfo::Class;
}
