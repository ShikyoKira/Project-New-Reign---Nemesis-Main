#pragma once

#include "Havok/Base/hkaAnimation.h"

namespace nemesis
{
    struct hkaAnimationBinding : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0x66eac971,
                                                "hkaAnimationBinding",
                                                &nemesis::hkReferencedObject::Class,
                                                72,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkaAnimationBinding);

        using BlendHint = nemesis::hkInternalClassEnumItem<char>;

        nemesis::hkStringPtr m_originalSkeletonName;
        nemesis::hkRefPtr<nemesis::hkaAnimation> m_animation;
        nemesis::hkArray<short> m_transformTrackToBoneIndices;
        nemesis::hkArray<short> m_floatTrackToFloatSlotIndices;
        nemesis::hkArray<short> m_partitionIndices;
        nemesis::hkEnum<BlendHint, char> m_blendHint = {
            {"NORMAL", 0},
            {"ADDITIVE", 1},
        };

    public:
        hkaAnimationBinding() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkaAnimationBinding::Class;
}
