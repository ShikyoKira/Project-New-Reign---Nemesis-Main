#pragma once

#include "Havok/Base/hkbModifier.h"

namespace nemesis
{
    struct BSLimbIKModifier : nemesis::hkbModifier
    {
        static constexpr nemesis::hkClass Class{0x8ea971e5,
                                                "BSLimbIKModifier",
                                                &nemesis::hkbModifier::Class,
                                                120,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(BSLimbIKModifier);

        float m_limitAngleDegrees{};
        float m_currentAngle{};
        short m_startBoneIndex{};
        short m_endBoneIndex{};
        float m_gain{};
        float m_boneRadius{};
        float m_castOffset{};
        float m_timeStep{};
        nemesis::hkRefPtr<nemesis::HavokObject> m_pSkeletonMemory;

    public:
        BSLimbIKModifier() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::BSLimbIKModifier::Class;
}
