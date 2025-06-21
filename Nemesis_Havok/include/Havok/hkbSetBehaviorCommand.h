#pragma once

#include "Havok/hkbBehaviorGraph.h"

namespace nemesis
{
    struct hkbSetBehaviorCommand : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0xe18b74b9,
                                                "hkbSetBehaviorCommand",
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
        REGISTER_HAVOK_POINTER_HEADER(hkbSetBehaviorCommand);

        uint64_t m_characterId;
        nemesis::hkRefPtr<nemesis::hkbBehaviorGraph> m_behavior;
        nemesis::hkRefPtr<nemesis::hkbGenerator> m_rootGenerator;
        nemesis::hkArray<nemesis::hkRefPtr<nemesis::hkbBehaviorGraph>> m_referencedBehaviors;
        int m_startStateIndex;
        bool m_randomizeSimulation;
        int m_padding;

    public:
        hkbSetBehaviorCommand() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbSetBehaviorCommand::Class;
}
