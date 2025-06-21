#pragma once

#include "Havok/Base/hkReferencedObject.h"

#include "Havok/hkaSkeletonMapper.h"
#include "Havok/hkbCharacterData.h"

namespace nemesis
{
    struct hkbCharacterSetup : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0xe5a2a413,
                                                "hkbCharacterSetup",
                                                &nemesis::hkReferencedObject::Class,
                                                88,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkbCharacterSetup);

        nemesis::hkArray<nemesis::hkRefPtr<nemesis::hkaSkeletonMapper>> m_retargetingSkeletonMappers;
        nemesis::hkRefPtr<nemesis::hkaSkeleton> m_animationSkeleton;
        nemesis::hkRefPtr<nemesis::hkaSkeleton> m_ragdollSkeleton;
        nemesis::hkRefPtr<nemesis::hkaSkeletonMapper> m_ragdollToAnimationSkeletonMapper;
        nemesis::hkRefPtr<nemesis::hkaSkeletonMapper> m_animationToRagdollSkeletonMapper;
        nemesis::hkRefPtr<nemesis::HavokObject> m_animationBindingSet;
        nemesis::hkRefPtr<nemesis::hkbCharacterData> m_data;
        nemesis::hkRefPtr<nemesis::HavokObject> m_mirroredSkeleton;
        nemesis::hkRefPtr<nemesis::HavokObject> m_characterPropertyIdMap;

    public:
        hkbCharacterSetup() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbCharacterSetup::Class;
}
