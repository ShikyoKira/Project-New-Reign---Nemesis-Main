#pragma once

#include "Havok/Base/hkxAttributeHolder.h"

#include "Havok/hkxMaterialProperty.h"
#include "Havok/hkxMaterialTextureStage.h"

namespace nemesis
{
    struct hkxMaterial : nemesis::hkxAttributeHolder
    {
        static constexpr nemesis::hkClass Class{0x2954537a,
                                                "hkxMaterial",
                                                &nemesis::hkxAttributeHolder::Class,
                                                176,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkxMaterial);

        using UVMappingAlgorithm = nemesis::hkInternalClassEnumItem<unsigned int>;
        using Transparency = nemesis::hkInternalClassEnumItem<unsigned char>;

        nemesis::hkStringPtr m_name;
        nemesis::hkArray<nemesis::hkxMaterialTextureStage> m_stages;
        nemesis::hkVector4 m_diffuseColor;
        nemesis::hkVector4 m_ambientColor;
        nemesis::hkVector4 m_specularColor;
        nemesis::hkVector4 m_emissiveColor;
        nemesis::hkArray<nemesis::hkRefPtr<nemesis::hkxMaterial>> m_subMaterials;
        nemesis::hkRefPtr<nemesis::hkReferencedObject> m_extraData;
        std::array<float, 2> m_uvMapScale{};
        std::array<float, 2> m_uvMapOffset{};
        float m_uvMapRotation                                              = 0.0f;
        nemesis::hkEnum<UVMappingAlgorithm, unsigned int> m_uvMapAlgorithm = {
            {"UVMA_SRT", 0},
            {"UVMA_TRS", 1},
            {"UVMA_3DSMAX_STYLE", 2},
            {"UVMA_MAYA_STYLE", 3},
        };
        float m_specularMultiplier                                  = 0.0f;
        float m_specularExponent                                    = 0.0f;
        nemesis::hkEnum<Transparency, unsigned char> m_transparency = {
            {"transp_none", 0},
            {"transp_alpha", 2},
            {"transp_additive", 3},
            {"transp_colorkey", 4},
            {"transp_subtractive", 9},
        };
        nemesis::hkUlong m_userData;
        nemesis::hkArray<nemesis::hkxMaterialProperty> m_properties;

    public:
        hkxMaterial() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkxMaterial::Class;
}
