#pragma once

#include "Havok/hkClass.h"

#include "Serialize/Deserializer.h"

namespace nemesis
{
    struct hkMotionState;

    struct hkSweptTransform : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0xb4e5770,
                                                "hkSweptTransform",
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
        REGISTER_HAVOK_POINTER_HEADER(hkSweptTransform);

        nemesis::hkVector4 m_centerOfMass0;
        nemesis::hkVector4 m_centerOfMass1;
        nemesis::hkQuaternion m_rotation0;
        nemesis::hkQuaternion m_rotation1;
        nemesis::hkVector4 m_centerOfMassLocal;

    public:
        hkSweptTransform() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;

        friend nemesis::hkMotionState;
    };

    constexpr nemesis::hkClass nemesis::hkSweptTransform::Class;
}
