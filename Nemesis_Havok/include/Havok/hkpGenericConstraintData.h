#pragma once

#include "Havok/Base/hkpConstraintData.h"

#include "Havok/hkpBridgeAtoms.h"
#include "Havok/hkpGenericConstraintDataScheme.h"

namespace nemesis
{
    struct hkpGenericConstraintData : nemesis::hkpConstraintData
    {
        static constexpr nemesis::hkClass Class{0xfa824640,
                                                "hkpGenericConstraintData",
                                                &nemesis::hkpConstraintData::Class,
                                                128,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpGenericConstraintData);

        nemesis::hkpBridgeAtoms m_atoms;
        nemesis::hkpGenericConstraintDataScheme m_scheme;

    public:
        hkpGenericConstraintData() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpGenericConstraintData::Class;
}
