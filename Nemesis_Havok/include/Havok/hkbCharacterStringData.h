#pragma once

#include "Havok/Base/hkReferencedObject.h"

#include "Havok/hkbAssetBundleStringData.h"
#include "Havok/hkbCharacterStringDataFileNameMeshNamePair.h"

namespace nemesis
{
    struct hkbCharacterStringData : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0x655b42bc,
                                                "hkbCharacterStringData",
                                                &nemesis::hkReferencedObject::Class,
                                                192,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkbCharacterStringData);

        nemesis::hkArray<nemesis::hkbCharacterStringDataFileNameMeshNamePair> m_skinNames;
        nemesis::hkArray<nemesis::hkbCharacterStringDataFileNameMeshNamePair> m_boneAttachmentNames;
        nemesis::hkArray<nemesis::hkbAssetBundleStringData> m_animationBundleNameData;
        nemesis::hkArray<nemesis::hkbAssetBundleStringData> m_animationBundleFilenameData;
        nemesis::hkArray<nemesis::hkCString> m_deformableSkinNames;
        nemesis::hkArray<nemesis::hkCString> m_rigidSkinNames;
        nemesis::hkArray<nemesis::hkCString> m_animationNames;
        nemesis::hkArray<nemesis::hkCString> m_animationFilenames;
        nemesis::hkArray<nemesis::hkCString> m_characterPropertyNames;
        nemesis::hkArray<nemesis::hkCString> m_retargetingSkeletonMapperFilenames;
        nemesis::hkArray<nemesis::hkCString> m_lodNames;
        nemesis::hkArray<nemesis::hkCString> m_mirroredSyncPointSubstringsA;
        nemesis::hkArray<nemesis::hkCString> m_mirroredSyncPointSubstringsB;
        nemesis::hkStringPtr m_name;
        nemesis::hkStringPtr m_rigName;
        nemesis::hkStringPtr m_ragdollName;
        nemesis::hkStringPtr m_behaviorFilename;
        nemesis::hkStringPtr m_luaScriptOnCharacterActivated;
        nemesis::hkStringPtr m_luaScriptOnCharacterDeactivated;
        nemesis::hkArray<nemesis::hkCString> m_luaFiles;

    public:
        hkbCharacterStringData() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbCharacterStringData::Class;
}
