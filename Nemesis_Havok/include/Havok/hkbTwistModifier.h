#pragma once

#include "Havok/Base/hkbModifier.h"

namespace nemesis
{
    struct hkbTwistModifier : nemesis::hkbModifier
    {
        static constexpr nemesis::hkClass Class{0xb6b76b32,
                                                "hkbTwistModifier",
                                                &nemesis::hkbModifier::Class,
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
        REGISTER_HAVOK_POINTER_HEADER(hkbTwistModifier);

        using SetAngleMethod = nemesis::hkInternalClassEnumItem<char>;
        using RotationAxisCoordinates = nemesis::hkInternalClassEnumItem<char>;

        nemesis::hkVector4 m_axisOfRotation;
        float m_twistAngle;
        short m_startBoneIndex;
        short m_endBoneIndex;
        nemesis::hkEnum<SetAngleMethod, char> m_setAngleMethod = {
            {"LINEAR", 0},
            {"RAMPED", 1},
        };
        nemesis::hkEnum<RotationAxisCoordinates, char> m_rotationAxisCoordinates = {
            {"ROTATION_AXIS_IN_MODEL_COORDINATES", 0},
            {"ROTATION_AXIS_IN_LOCAL_COORDINATES", 1},
        };
        bool m_isAdditive;
        nemesis::hkArray<nemesis::hkRefPtr<nemesis::HavokObject>> m_boneChainIndices;
        nemesis::hkArray<nemesis::hkRefPtr<nemesis::HavokObject>> m_parentBoneIndices;

    public:
        hkbTwistModifier() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbTwistModifier::Class;
}
