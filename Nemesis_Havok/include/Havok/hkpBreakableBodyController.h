#pragma once

#include "Havok/Base/hkReferencedObject.h"

namespace nemesis
{
    struct hkpBreakableBodyController : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0x00,
                                                "hkpBreakableBodyController",
                                                &nemesis::hkReferencedObject::Class,
                                                20,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpBreakableBodyController);

        float m_breakingImpulse;

    public:
        hkpBreakableBodyController() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpBreakableBodyController::Class;
}
