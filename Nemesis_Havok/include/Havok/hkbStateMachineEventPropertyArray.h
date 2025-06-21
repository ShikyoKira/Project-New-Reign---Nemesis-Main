#pragma once

#include "Havok/Base/hkReferencedObject.h"

#include "Havok/hkArray.h"
#include "Havok/hkbEventProperty.h"

namespace nemesis
{
    struct hkbStateMachineEventPropertyArray : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0xb07b4388,
                                                "hkbStateMachineEventPropertyArray",
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
        REGISTER_HAVOK_POINTER_HEADER(hkbStateMachineEventPropertyArray);

        nemesis::hkArray<nemesis::hkbEventProperty> m_events;

    public:
        hkbStateMachineEventPropertyArray() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbStateMachineEventPropertyArray::Class;
}
