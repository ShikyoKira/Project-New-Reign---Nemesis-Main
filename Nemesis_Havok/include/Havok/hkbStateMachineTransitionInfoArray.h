#pragma once

#include "Havok/Base/hkReferencedObject.h"

#include "Havok/hkbStateMachineTransitionInfo.h"

namespace nemesis
{
    struct hkbStateMachineTransitionInfoArray : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0xe397b11e,
                                                "hkbStateMachineTransitionInfoArray",
                                                &nemesis::hkReferencedObject::Class,
                                                32,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkbStateMachineTransitionInfoArray);

        nemesis::hkArray<nemesis::hkbStateMachineTransitionInfo> m_transitions;

    public:
        hkbStateMachineTransitionInfoArray() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbStateMachineTransitionInfoArray::Class;
}
