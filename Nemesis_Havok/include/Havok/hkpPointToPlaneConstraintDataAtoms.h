#pragma once

#include "Havok/hkpSetLocalTransformsConstraintAtom.h"
#include "Havok/hkpLinConstraintAtom.h"

namespace nemesis
{
    struct hkpPointToPlaneConstraintDataAtoms : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0x749bc260,
                                                "hkpPointToPlaneConstraintDataAtoms",
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
        REGISTER_HAVOK_POINTER_HEADER(hkpPointToPlaneConstraintDataAtoms);

        nemesis::hkpSetLocalTransformsConstraintAtom m_transforms;
        nemesis::hkpLinConstraintAtom m_lin;

    public:
        hkpPointToPlaneConstraintDataAtoms() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpPointToPlaneConstraintDataAtoms::Class;
}
