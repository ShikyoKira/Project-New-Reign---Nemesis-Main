#pragma once

#include "Havok/hkpSetLocalTranslationsConstraintAtom.h"
#include "Havok/hkpPulleyConstraintAtom.h"

namespace nemesis
{
    struct hkpPulleyConstraintDataAtoms : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0xb149e5a,
                                                "hkpPulleyConstraintDataAtoms",
                                                nullptr,
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
        REGISTER_HAVOK_POINTER_HEADER(hkpPulleyConstraintDataAtoms);

        nemesis::hkpSetLocalTranslationsConstraintAtom m_translations;
        nemesis::hkpPulleyConstraintAtom m_pulley;

    public:
        hkpPulleyConstraintDataAtoms() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpPulleyConstraintDataAtoms::Class;
}
