#pragma once

#include "Havok/Base/hkpConstraintAtom.h"

namespace nemesis
{
    struct hkpModifierConstraintAtom : nemesis::hkpConstraintAtom
    {
        static constexpr nemesis::hkClass Class{0,
                                                "hkpModifierConstraintAtom",
                                                &nemesis::hkpConstraintAtom::Class,
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
        REGISTER_HAVOK_POINTER_HEADER(hkpModifierConstraintAtom);

        unsigned short m_modifierAtomSize;
        unsigned short m_childSize;
        nemesis::hkRefPtr<nemesis::hkpConstraintAtom> m_child;
        std::array<unsigned int, 2> m_pad;

    public:
        hkpModifierConstraintAtom() noexcept;
        hkpModifierConstraintAtom(const nemesis::hkClass& cls) noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpModifierConstraintAtom::Class;
}
