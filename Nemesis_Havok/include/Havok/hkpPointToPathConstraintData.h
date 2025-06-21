#pragma once

#include "Havok/Base/hkpConstraintData.h"
#include "Havok/Base/hkpParametricCurve.h"

#include "Havok/hkpBridgeAtoms.h"

namespace nemesis
{
    struct hkpPointToPathConstraintData : nemesis::hkpConstraintData
    {
        static constexpr nemesis::hkClass Class{0x8e7cb5da,
                                                "hkpPointToPathConstraintData",
                                                &nemesis::hkpConstraintData::Class,
                                                192,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpPointToPathConstraintData);

        using OrientationConstraintType = nemesis::hkInternalClassEnumItem<char>;

        nemesis::hkpBridgeAtoms m_atoms;
        nemesis::hkRefPtr<nemesis::hkpParametricCurve> m_path;
        float m_maxFrictionForce;
        nemesis::hkEnum<OrientationConstraintType, char> m_angularConstrainedDOF = {
            {"CONSTRAIN_ORIENTATION_INVALID", 0},
            {"CONSTRAIN_ORIENTATION_NONE", 1},
            {"CONSTRAIN_ORIENTATION_ALLOW_SPIN", 2},
            {"CONSTRAIN_ORIENTATION_TO_PATH", 3},
            {"CONSTRAIN_ORIENTATION_MAX_ID", 4},
        };
        std::array<nemesis::hkTransform, 2> m_transform_OS_KS;

    public:
        hkpPointToPathConstraintData() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpPointToPathConstraintData::Class;
}
