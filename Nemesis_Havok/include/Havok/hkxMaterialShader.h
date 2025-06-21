#pragma once

#include "Havok/Base/hkReferencedObject.h"

namespace nemesis
{
    struct hkxMaterialShader : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0x28515eff,
                                                "hkxMaterialShader",
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
        REGISTER_HAVOK_POINTER_HEADER(hkxMaterialShader);

        using ShaderType = nemesis::hkInternalClassEnumItem<unsigned char>;

        nemesis::hkStringPtr m_name;
        nemesis::hkEnum<ShaderType, unsigned char> m_type = {
            {"EFFECT_TYPE_INVALID", 0},
            {"EFFECT_TYPE_UNKNOWN", 1},
            {"EFFECT_TYPE_HLSL_INLINE", 2},
            {"EFFECT_TYPE_CG_INLINE", 3},
            {"EFFECT_TYPE_HLSL_FILENAME", 4},
            {"EFFECT_TYPE_CG_FILENAME", 5},
            {"EFFECT_TYPE_MAX_ID", 6},
        };
        nemesis::hkStringPtr m_vertexEntryName;
        nemesis::hkStringPtr m_geomEntryName;
        nemesis::hkStringPtr m_pixelEntryName;
        nemesis::hkArray<unsigned char> m_data;

    public:
        hkxMaterialShader() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkxMaterialShader::Class;
}
