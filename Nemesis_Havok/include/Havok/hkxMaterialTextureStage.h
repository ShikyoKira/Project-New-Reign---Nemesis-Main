#pragma once

#include "Havok/hkClass.h"

#include "Havok/Base/hkReferencedObject.h"

namespace nemesis
{
    struct hkxMaterialTextureStage : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0xfa6facb2,
                                                "hkxMaterialTextureStage",
                                                nullptr,
                                                16,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkxMaterialTextureStage);

        using TextureType = nemesis::hkInternalClassEnumItem<int>;

        nemesis::hkRefPtr<nemesis::hkReferencedObject> m_texture;
        nemesis::hkEnum<TextureType, int> m_usageHint = {
            {"TEX_UNKNOWN", 0},
            {"TEX_DIFFUSE", 1},
            {"TEX_REFLECTION", 2},
            {"TEX_BUMP", 3},
            {"TEX_NORMAL", 4},
            {"TEX_DISPLACEMENT", 5},
            {"TEX_SPECULAR", 6},
            {"TEX_SPECULARANDGLOSS", 7},
            {"TEX_OPACITY", 8},
            {"TEX_EMISSIVE", 9},
            {"TEX_REFRACTION", 10},
            {"TEX_GLOSS", 11},
            {"TEX_DOMINANTS", 12},
            {"TEX_NOTEXPORTED", 13},
        };
        int m_tcoordChannel;

    public:
        hkxMaterialTextureStage() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkxMaterialTextureStage::Class;
}
