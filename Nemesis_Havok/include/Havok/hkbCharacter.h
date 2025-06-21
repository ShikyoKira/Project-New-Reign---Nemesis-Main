#pragma once

#include "Havok/Base/hkReferencedObject.h"

#include "Havok/hkbBehaviorGraph.h"
#include "Havok/hkbCharacterSetup.h"
#include "Havok/hkbProjectData.h"

namespace nemesis
{
    struct hkbCharacter : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0x3088a5c5,
                                                "hkbCharacter",
                                                &nemesis::hkReferencedObject::Class,
                                                160,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkbCharacter);

        nemesis::hkArray<nemesis::hkRefPtr<nemesis::hkbCharacter>> m_nearbyCharacters;
        nemesis::hkUlong m_userData;
        short m_currentLod;
        short m_numTracksInLod;
        nemesis::hkStringPtr m_name;
        nemesis::hkRefPtr<nemesis::HavokObject> m_ragdollDriver;
        nemesis::hkRefPtr<nemesis::HavokObject> m_characterControllerDriver;
        nemesis::hkRefPtr<nemesis::HavokObject> m_footIkDriver;
        nemesis::hkRefPtr<nemesis::HavokObject> m_handIkDriver;
        nemesis::hkRefPtr<nemesis::hkbCharacterSetup> m_setup;
        nemesis::hkRefPtr<nemesis::hkbBehaviorGraph> m_behaviorGraph;
        nemesis::hkRefPtr<nemesis::hkbProjectData> m_projectData;
        nemesis::hkRefPtr<nemesis::HavokObject> m_animationBindingSet;
        nemesis::hkRefPtr<nemesis::HavokObject> m_raycastInterface;
        nemesis::hkRefPtr<nemesis::HavokObject> m_world;
        nemesis::hkRefPtr<nemesis::HavokObject> m_eventQueue;
        nemesis::hkRefPtr<nemesis::HavokObject> m_worldFromModel;
        nemesis::hkRefPtr<nemesis::HavokObject> m_poseLocal;
        int m_numPoseLocal;
        int m_capabilities;
        int m_effectiveCapabilities;
        bool m_deleteWorldFromModel;
        bool m_deletePoseLocal;

    public:
        hkbCharacter() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbCharacter::Class;
}
