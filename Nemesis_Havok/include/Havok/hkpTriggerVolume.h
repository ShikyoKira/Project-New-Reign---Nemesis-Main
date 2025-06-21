#pragma once

#include "Havok/Base/hkpContactListener.h"
#include "Havok/Base/hkpEntityListener.h"
#include "Havok/Base/hkpWorldPostSimulationListener.h"

#include "Havok/hkpRigidBody.h"
#include "Havok/hkpTriggerVolumeEventInfo.h"

namespace nemesis
{
    struct hkpTriggerVolume : nemesis::hkReferencedObject,
                              nemesis::hkpContactListener,
                              nemesis::hkpWorldPostSimulationListener,
                              nemesis::hkpEntityListener
    {
        static constexpr nemesis::hkClass Class{0xa29a8d1a,
                                                "hkpTriggerVolume",
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
        REGISTER_HAVOK_POINTER_HEADER(hkpTriggerVolume);

        nemesis::hkArray<nemesis::hkRefPtr<nemesis::hkpRigidBody>> m_overlappingBodies;
        nemesis::hkArray<nemesis::hkpTriggerVolumeEventInfo> m_eventQueue;
        nemesis::hkRefPtr<nemesis::hkpRigidBody> m_triggerBody;
        unsigned int m_sequenceNumber;

    public:
        hkpTriggerVolume() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpTriggerVolume::Class;
}
