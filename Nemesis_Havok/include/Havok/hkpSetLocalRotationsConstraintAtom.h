#pragma once

#include "Havok/Base/hkpConstraintAtom.h"

namespace nemesis
{
    struct hkpSetLocalRotationsConstraintAtom : nemesis::hkpConstraintAtom
    {
        static constexpr nemesis::hkClass Class{0xf81db8e,
                                                "hkpSetLocalRotationsConstraintAtom",
                                                &nemesis::hkpConstraintAtom::Class,
                                                112,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpSetLocalRotationsConstraintAtom);

        nemesis::hkRotation m_rotationA;
        nemesis::hkRotation m_rotationB;

    public:
        hkpSetLocalRotationsConstraintAtom() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpSetLocalRotationsConstraintAtom::Class;
}
