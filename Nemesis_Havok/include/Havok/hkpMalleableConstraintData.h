#pragma once

#include "Havok/Base/hkpWrappedConstraintData.h"

#include "Havok/hkpBridgeAtoms.h"

namespace nemesis
{
    struct hkpMalleableConstraintData : nemesis::hkpWrappedConstraintData
    {
        static constexpr nemesis::hkClass Class{0x6748b2cf,
                                                "hkpMalleableConstraintData",
                                                &nemesis::hkpWrappedConstraintData::Class,
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
        REGISTER_HAVOK_POINTER_HEADER(hkpMalleableConstraintData);

        nemesis::hkRefPtr<nemesis::hkpConstraintData> m_constraintData;
        nemesis::hkpBridgeAtoms m_atoms;
        float m_strength;

    public:
        hkpMalleableConstraintData() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpMalleableConstraintData::Class;
}
