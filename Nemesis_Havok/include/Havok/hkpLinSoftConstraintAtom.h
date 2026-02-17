#pragma once

#include "Havok/Base/hkpConstraintAtom.h"

namespace nemesis
{
    struct hkpLinSoftConstraintAtom : nemesis::hkpConstraintAtom
    {
        static constexpr nemesis::hkClass Class{0x52b27d69,
                                                "hkpLinSoftConstraintAtom",
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
        REGISTER_HAVOK_POINTER_HEADER(hkpLinSoftConstraintAtom);

        unsigned char m_axisIndex{};
        float m_tau{};
        float m_damping{};

    public:
        hkpLinSoftConstraintAtom() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpLinSoftConstraintAtom::Class;
}
