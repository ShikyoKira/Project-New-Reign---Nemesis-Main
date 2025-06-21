#pragma once

#include "Havok/hkClass.h"

#include "Serialize/Deserializer.h"

namespace nemesis
{
    struct hkbStateMachineNestedStateMachineData : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0x7358f5da,
                                                "hkbStateMachineNestedStateMachineData",
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
        REGISTER_HAVOK_POINTER_HEADER(hkbStateMachineNestedStateMachineData);

        nemesis::hkRefPtr<nemesis::HavokObject> m_nestedStateMachine;
        nemesis::hkRefPtr<nemesis::HavokObject> m_eventIdMap;

    public:
        hkbStateMachineNestedStateMachineData() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbStateMachineNestedStateMachineData::Class;
}
