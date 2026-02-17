#pragma once

#include "Havok/Base/hkpConstraintAtom.h"

namespace nemesis
{
    struct hkpAngConstraintAtom : nemesis::hkpConstraintAtom
    {
        static constexpr nemesis::hkClass Class{0x35bb3cd0,
                                                "hkpAngConstraintAtom",
                                                &nemesis::hkpConstraintAtom::Class,
                                                4,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpAngConstraintAtom);

        unsigned char m_firstConstrainedAxis{};
        unsigned char m_numConstrainedAxes{};

    public:
        hkpAngConstraintAtom() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpAngConstraintAtom::Class;
}
