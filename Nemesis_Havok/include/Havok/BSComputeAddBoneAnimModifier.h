#pragma once

#include "Havok/Base/hkbModifier.h"

namespace nemesis
{
    struct BSComputeAddBoneAnimModifier : nemesis::hkbModifier
    {
        static constexpr nemesis::hkClass Class{0xa67f8c46,
                                                "BSComputeAddBoneAnimModifier",
                                                &nemesis::hkbModifier::Class,
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
        REGISTER_HAVOK_POINTER_HEADER(BSComputeAddBoneAnimModifier);

        short m_boneIndex;
        nemesis::hkVector4 m_translationLSOut;
        nemesis::hkQuaternion m_rotationLSOut;
        nemesis::hkVector4 m_scaleLSOut;
        nemesis::hkRefPtr<nemesis::HavokObject> m_pSkeletonMemory;

    public:
        BSComputeAddBoneAnimModifier() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::BSComputeAddBoneAnimModifier::Class;
}
