#pragma once

#include "Havok/Base/hkpConstraintAtom.h"

namespace nemesis
{
    struct hkpSetLocalTranslationsConstraintAtom : nemesis::hkpConstraintAtom
    {
        static constexpr nemesis::hkClass Class{0x5cbfcf4a,
                                                "hkpSetLocalTranslationsConstraintAtom",
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
        REGISTER_HAVOK_POINTER_HEADER(hkpSetLocalTranslationsConstraintAtom);

        nemesis::hkVector4 m_translationA;
        nemesis::hkVector4 m_translationB;

    public:
        hkpSetLocalTranslationsConstraintAtom() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpSetLocalTranslationsConstraintAtom::Class;
}
