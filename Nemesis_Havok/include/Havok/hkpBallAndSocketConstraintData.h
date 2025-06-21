#pragma once

#include "Havok/Base/hkpConstraintData.h"

#include "Havok/hkpBallAndSocketConstraintDataAtoms.h"

namespace nemesis
{
    struct hkpBallAndSocketConstraintData : nemesis::hkpConstraintData
    {
        static constexpr nemesis::hkClass Class{0x5a6954d9,
                                                "hkpBallAndSocketConstraintData",
                                                &nemesis::hkpConstraintData::Class,
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
        REGISTER_HAVOK_POINTER_HEADER(hkpBallAndSocketConstraintData);

        nemesis::hkpBallAndSocketConstraintDataAtoms m_atoms; // align 16

    public:
        hkpBallAndSocketConstraintData() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpBallAndSocketConstraintData::Class;
}
