#pragma once

#include "Havok/Base/hkReferencedObject.h"

namespace nemesis
{
    struct hkxMaterialEffect : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0x1d39f925,
                                                "hkxMaterialEffect",
                                                &nemesis::hkReferencedObject::Class,
                                                48,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkxMaterialEffect);

        using EffectType = nemesis::hkInternalClassEnumItem<unsigned char>;

        nemesis::hkStringPtr m_name;
        nemesis::hkEnum<EffectType, unsigned char> m_type = {
            {"EFFECT_TYPE_INVALID", 0},
            {"EFFECT_TYPE_UNKNOWN", 1},
            {"EFFECT_TYPE_HLSL_FX_INLINE", 2},
            {"EFFECT_TYPE_CG_FX_INLINE", 3},
            {"EFFECT_TYPE_HLSL_FX_FILENAME", 4},
            {"EFFECT_TYPE_CG_FX_FILENAME", 5},
            {"EFFECT_TYPE_MAX_ID", 6},
        };
        nemesis::hkArray<unsigned char> m_data;

    public:
        hkxMaterialEffect() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkxMaterialEffect::Class;
}
