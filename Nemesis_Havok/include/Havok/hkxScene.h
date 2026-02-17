#pragma once

#include "Havok/Base/hkReferencedObject.h"

#include "Havok/hkxNode.h"
#include "Havok/hkxNodeSelectionSet.h"
#include "Havok/hkxCamera.h"
#include "Havok/hkxLight.h"
#include "Havok/hkxMesh.h"
#include "Havok/hkxMaterial.h"
#include "Havok/hkxTextureInplace.h"
#include "Havok/hkxTextureFile.h"
#include "Havok/hkxSkinBinding.h"
#include "Havok/hkxSpline.h"

namespace nemesis
{
    struct hkxScene : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0x5f673ddd,
                                                "hkxScene",
                                                &nemesis::hkReferencedObject::Class,
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
        REGISTER_HAVOK_POINTER_HEADER(hkxScene);

        nemesis::hkStringPtr m_modeller;
        nemesis::hkStringPtr m_asset;
        float m_sceneLength{};
        unsigned int m_numFrames{};
        nemesis::hkRefPtr<nemesis::hkxNode> m_rootNode;
        nemesis::hkArray<nemesis::hkRefPtr<nemesis::hkxNodeSelectionSet>> m_selectionSets;
        nemesis::hkArray<nemesis::hkRefPtr<nemesis::hkxCamera>> m_cameras;
        nemesis::hkArray<nemesis::hkRefPtr<nemesis::hkxLight>> m_lights;
        nemesis::hkArray<nemesis::hkRefPtr<nemesis::hkxMesh>> m_meshes;
        nemesis::hkArray<nemesis::hkRefPtr<nemesis::hkxMaterial>> m_materials;
        nemesis::hkArray<nemesis::hkRefPtr<nemesis::hkxTextureInplace>> m_inplaceTextures;
        nemesis::hkArray<nemesis::hkRefPtr<nemesis::hkxTextureFile>> m_externalTextures;
        nemesis::hkArray<nemesis::hkRefPtr<nemesis::hkxSkinBinding>> m_skinBindings;
        nemesis::hkArray<nemesis::hkRefPtr<nemesis::hkxSpline>> m_splines;
        nemesis::hkMatrix3 m_appliedTransform;

    public:
        hkxScene() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkxScene::Class;
}
