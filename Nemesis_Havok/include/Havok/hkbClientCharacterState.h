#pragma once

#include "Havok/hkaSkeleton.h"
#include "Havok/hkbAuxiliaryNodeInfo.h"
#include "Havok/hkbBehaviorGraphData.h"
#include "Havok/hkbBehaviorGraphInternalState.h"

namespace nemesis
{
    struct hkbClientCharacterState : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0xa2624c97,
                                                "hkbClientCharacterState",
                                                &nemesis::hkReferencedObject::Class,
                                                272,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkbClientCharacterState);

        nemesis::hkArray<uint64_t> m_deformableSkinIds;
        nemesis::hkArray<uint64_t> m_rigidSkinIds;
        nemesis::hkArray<short> m_externalEventIds;
        nemesis::hkArray<nemesis::hkRefPtr<nemesis::hkbAuxiliaryNodeInfo>> m_auxiliaryInfo;
        nemesis::hkArray<short> m_activeEventIds;
        nemesis::hkArray<short> m_activeVariableIds;
        uint64_t m_characterId{};
        nemesis::hkStringPtr m_instanceName;
        nemesis::hkStringPtr m_templateName;
        nemesis::hkStringPtr m_fullPathToProject;
        nemesis::hkStringPtr m_localScriptsPath;
        nemesis::hkStringPtr m_remoteScriptsPath;
        nemesis::hkRefPtr<nemesis::hkbBehaviorGraphData> m_behaviorData;
        nemesis::hkRefPtr<nemesis::hkbBehaviorGraphInternalState> m_behaviorInternalState;
        nemesis::hkRefPtr<nemesis::HavokObject> m_nodeIdToInternalStateMap;
        bool m_visible{};
        float m_elapsedSimulationTime{};
        nemesis::hkRefPtr<nemesis::hkaSkeleton> m_skeleton;
        nemesis::hkQsTransform m_worldFromModel;
        nemesis::hkArray<nemesis::hkQsTransform> m_poseModelSpace;
        nemesis::hkArray<nemesis::hkQsTransform> m_rigidAttachmentTransforms;

    public:
        hkbClientCharacterState() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbClientCharacterState::Class;
}
