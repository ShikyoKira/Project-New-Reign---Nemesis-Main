#pragma once

#include "Havok/Base/hkReferencedObject.h"

namespace nemesis
{
    struct hkbComputeDirectionModifierInternalState : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0x6ac054d7,
                                                "hkbComputeDirectionModifierInternalState",
                                                &nemesis::hkReferencedObject::Class,
                                                48,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkbComputeDirectionModifierInternalState);

        nemesis::hkVector4 m_pointOut;
        float m_groundAngleOut{};
        float m_upAngleOut{};
        bool m_computedOutput{};

    public:
        hkbComputeDirectionModifierInternalState() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbComputeDirectionModifierInternalState::Class;
}
