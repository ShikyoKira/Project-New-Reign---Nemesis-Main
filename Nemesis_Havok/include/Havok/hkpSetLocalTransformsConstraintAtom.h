#pragma once

#include "Havok/Base/hkpConstraintAtom.h"

namespace nemesis
{
    struct hkpSetLocalTransformsConstraintAtom : nemesis::hkpConstraintAtom
    {
        static constexpr nemesis::hkClass Class{0x6e2a5198,
                                                "hkpSetLocalTransformsConstraintAtom",
                                                &nemesis::hkpConstraintAtom::Class,
                                                144,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpSetLocalTransformsConstraintAtom);

        nemesis::hkTransform m_transformA;
        nemesis::hkTransform m_transformB;

    public:
        hkpSetLocalTransformsConstraintAtom() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpSetLocalTransformsConstraintAtom::Class;
}
