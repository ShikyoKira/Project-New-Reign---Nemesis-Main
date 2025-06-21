#pragma once

#include "Havok/hkpModifierConstraintAtom.h"

namespace nemesis
{
    struct hkpIgnoreModifierConstraintAtom : nemesis::hkpModifierConstraintAtom
    {
        static constexpr nemesis::hkClass Class{0x5c6aa14d,
                                                "hkpIgnoreModifierConstraintAtom",
                                                &nemesis::hkpModifierConstraintAtom::Class,
                                                48,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpIgnoreModifierConstraintAtom);

    public:
        hkpIgnoreModifierConstraintAtom() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpIgnoreModifierConstraintAtom::Class;
}
