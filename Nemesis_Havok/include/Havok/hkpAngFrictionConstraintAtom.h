#pragma once

#include "Havok/Base/hkpConstraintAtom.h"

namespace nemesis
{
    struct hkpAngFrictionConstraintAtom : nemesis::hkpConstraintAtom
    {
        static constexpr nemesis::hkClass Class{0xf313aa80,
                                                "hkpAngFrictionConstraintAtom",
                                                &nemesis::hkpConstraintAtom::Class,
                                                12,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpAngFrictionConstraintAtom);

        unsigned char m_isEnabled{};
        unsigned char m_firstFrictionAxis{};
        unsigned char m_numFrictionAxes{};
        float m_maxFrictionTorque{};

    public:
        hkpAngFrictionConstraintAtom() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpAngFrictionConstraintAtom::Class;
}
