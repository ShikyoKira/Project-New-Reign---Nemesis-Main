#pragma once

#include "Havok/hkpSetLocalTransformsConstraintAtom.h"
#include "Havok/hkpRackAndPinionConstraintAtom.h"

namespace nemesis
{
    struct hkpRackAndPinionConstraintDataAtoms : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0xa58a9659,
                                                "hkpRackAndPinionConstraintDataAtoms",
                                                nullptr,
                                                160,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpRackAndPinionConstraintDataAtoms);

        nemesis::hkpSetLocalTransformsConstraintAtom m_transforms;
        nemesis::hkpRackAndPinionConstraintAtom m_rackAndPinion;

    public:
        hkpRackAndPinionConstraintDataAtoms() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpRackAndPinionConstraintDataAtoms::Class;
}
