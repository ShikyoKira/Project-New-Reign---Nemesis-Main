#pragma once

#include "Havok/hkbBehaviorGraph.h"
#include "Havok/hkbCharacter.h"
#include "Havok/hkbGeneratorOutputListener.h"
#include "Havok/hkbProjectData.h"

namespace nemesis
{
    struct hkbContext : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0xe0c4d4a7,
                                                "hkbContext",
                                                nullptr,
                                                80,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkbContext);

        nemesis::hkRefPtr<nemesis::hkbCharacter> m_character;
        nemesis::hkRefPtr<nemesis::hkbBehaviorGraph> m_rootBehavior;
        nemesis::hkRefPtr<nemesis::hkbBehaviorGraph> m_behavior;
        nemesis::hkRefPtr<nemesis::HavokObject> m_nodeToIndexMap;
        nemesis::hkRefPtr<nemesis::HavokObject> m_eventQueue;
        nemesis::hkRefPtr<nemesis::HavokObject> m_sharedEventQueue;
        nemesis::hkRefPtr<nemesis::hkbGeneratorOutputListener> m_generatorOutputListener;
        nemesis::hkRefPtr<nemesis::hkbProjectData> m_projectData;
        bool m_eventTriggeredTransition{};
        nemesis::hkRefPtr<nemesis::HavokObject> m_world;
        nemesis::hkRefPtr<nemesis::HavokObject> m_attachmentManager;
        nemesis::hkRefPtr<nemesis::HavokObject> m_animationCache;

    public:
        hkbContext() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbContext::Class;
}
