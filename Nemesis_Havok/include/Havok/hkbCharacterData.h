#pragma once

#include "Havok/hkbCharacterControllerSetup.h"
#include "Havok/hkbCharacterDataCharacterControllerInfo.h"
#include "Havok/hkbCharacterStringData.h"
#include "Havok/hkbFootIkDriverInfo.h"
#include "Havok/hkbHandIkDriverInfo.h"
#include "Havok/hkbMirroredSkeletonInfo.h"
#include "Havok/hkbVariableInfo.h"
#include "Havok/hkbVariableValueSet.h"

namespace nemesis
{
    struct hkbCharacterData : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0x300d6808,
                                                "hkbCharacterData",
                                                &nemesis::hkReferencedObject::Class,
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
        REGISTER_HAVOK_POINTER_HEADER(hkbCharacterData);

        nemesis::hkbCharacterDataCharacterControllerInfo m_characterControllerInfo;
        nemesis::hkbCharacterControllerSetup m_characterControllerSetup;
        nemesis::hkVector4 m_modelUpMS;
        nemesis::hkVector4 m_modelForwardMS;
        nemesis::hkVector4 m_modelRightMS;
        nemesis::hkArray<nemesis::hkbVariableInfo> m_characterPropertyInfos;
        nemesis::hkArray<int> m_numBonesPerLod;
        nemesis::hkRefPtr<nemesis::hkbVariableValueSet> m_characterPropertyValues;
        nemesis::hkRefPtr<nemesis::hkbFootIkDriverInfo> m_footIkDriverInfo;
        nemesis::hkRefPtr<nemesis::hkbHandIkDriverInfo> m_handIkDriverInfo;
        nemesis::hkRefPtr<nemesis::hkReferencedObject> m_aiControlDriverInfo;
        nemesis::hkRefPtr<nemesis::hkbCharacterStringData> m_stringData;
        nemesis::hkRefPtr<nemesis::hkbMirroredSkeletonInfo> m_mirroredSkeletonInfo;
        nemesis::hkArray<short> m_boneAttachmentBoneIndices;
        nemesis::hkArray<nemesis::hkMatrix4> m_boneAttachmentTransforms;
        float m_scale;
        short m_numHands;
        short m_numFloatSlots;

    public:
        hkbCharacterData() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbCharacterData::Class;
}
