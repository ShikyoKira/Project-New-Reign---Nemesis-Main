#pragma once

#include "Havok/hkpBreakableBodyController.h"
#include "Havok/hkpBreakableShape.h"

namespace nemesis
{
    struct hkpBreakableBody : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0xda8c7d7d,
                                                "hkpBreakableBody",
                                                &nemesis::hkReferencedObject::Class,
                                                40,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        nemesis::hkRefPtr<nemesis::hkpBreakableBodyController> m_controller;
        nemesis::hkRefPtr<nemesis::hkpBreakableShape> m_breakableShape;
        unsigned char m_bodyTypeAndFlags{};
        nemesis::hkHalf m_constraintStrength;

    public:
        hkpBreakableBody(const nemesis::hkClass& cls) noexcept;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpBreakableBody::Class;
}
