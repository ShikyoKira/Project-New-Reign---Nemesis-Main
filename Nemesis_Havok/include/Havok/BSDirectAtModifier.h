#pragma once

#include "Havok/Base/hkbModifier.h"

namespace nemesis
{
    struct BSDirectAtModifier : nemesis::hkbModifier
    {
        static constexpr nemesis::hkClass Class{0x19a005c0,
                                                "BSDirectAtModifier",
                                                &nemesis::hkbModifier::Class,
                                                224,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(BSDirectAtModifier);

        bool m_directAtTarget{};
        short m_sourceBoneIndex{};
        short m_startBoneIndex{};
        short m_endBoneIndex{};
        float m_limitHeadingDegrees{};
        float m_limitPitchDegrees{};
        float m_offsetHeadingDegrees{};
        float m_offsetPitchDegrees{};
        float m_onGain{};
        float m_offGain{};
        nemesis::hkVector4 m_targetLocation;
        unsigned int m_userInfo{};
        bool m_directAtCamera{};
        float m_directAtCameraX{};
        float m_directAtCameraY{};
        float m_directAtCameraZ{};
        bool m_active{};
        float m_currentHeadingOffset{};
        float m_currentPitchOffset{};
        float m_timeStep{};
        nemesis::hkRefPtr<nemesis::HavokObject> m_pSkeletonMemory;
        bool m_hasTarget{};
        nemesis::hkVector4 m_directAtTargetLocation;
        nemesis::hkArray<nemesis::hkRefPtr<nemesis::HavokObject>> m_boneChainIndices;

    public:
        BSDirectAtModifier() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::BSDirectAtModifier::Class;
}
