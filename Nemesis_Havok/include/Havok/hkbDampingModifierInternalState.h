#pragma once

#include "Havok/Base/hkReferencedObject.h"

namespace nemesis
{
    struct hkbDampingModifierInternalState : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0x508d3b36,
                                                "hkbDampingModifierInternalState",
                                                &nemesis::hkReferencedObject::Class,
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
        REGISTER_HAVOK_POINTER_HEADER(hkbDampingModifierInternalState);

        nemesis::hkVector4 m_dampedVector;
        nemesis::hkVector4 m_vecErrorSum;
        nemesis::hkVector4 m_vecPreviousError;
        float m_dampedValue{};
        float m_errorSum{};
        float m_previousError{};

    public:
        hkbDampingModifierInternalState() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbDampingModifierInternalState::Class;
}
