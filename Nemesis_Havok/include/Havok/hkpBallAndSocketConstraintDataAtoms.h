#pragma once

#include "Havok/hkpBallSocketConstraintAtom.h"
#include "Havok/hkpSetLocalTranslationsConstraintAtom.h"
#include "Havok/hkpSetupStabilizationAtom.h"

namespace nemesis
{
    struct hkpBallAndSocketConstraintDataAtoms : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0xc73dcaf9,
                                                "hkpBallAndSocketConstraintDataAtoms",
                                                nullptr,
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
        REGISTER_HAVOK_POINTER_HEADER(hkpBallAndSocketConstraintDataAtoms);

        nemesis::hkpSetLocalTranslationsConstraintAtom m_pivots;
        nemesis::hkpSetupStabilizationAtom m_setupStabilization;
        nemesis::hkpBallSocketConstraintAtom m_ballSocket;

    public:
        hkpBallAndSocketConstraintDataAtoms() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpBallAndSocketConstraintDataAtoms::Class;
}
