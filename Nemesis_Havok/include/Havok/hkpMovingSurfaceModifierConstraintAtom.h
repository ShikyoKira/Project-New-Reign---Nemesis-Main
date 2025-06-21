#pragma once

#include "Havok/hkpModifierConstraintAtom.h"

namespace nemesis
{
    struct hkpMovingSurfaceModifierConstraintAtom : nemesis::hkpModifierConstraintAtom
    {
        static constexpr nemesis::hkClass Class{0x79ab517d,
                                                "hkpMovingSurfaceModifierConstraintAtom",
                                                &nemesis::hkpModifierConstraintAtom::Class,
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
        REGISTER_HAVOK_POINTER_HEADER(hkpMovingSurfaceModifierConstraintAtom);

        nemesis::hkVector4 m_velocity;

    public:
        hkpMovingSurfaceModifierConstraintAtom() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpMovingSurfaceModifierConstraintAtom::Class;
}
