#pragma once

#include "Havok/Base/hkReferencedObject.h"

namespace nemesis
{
    struct hkxCamera : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0xe3597b02,
                                                "hkxCamera",
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
        REGISTER_HAVOK_POINTER_HEADER(hkxCamera);

        nemesis::hkVector4 m_from;
        nemesis::hkVector4 m_focus;
        nemesis::hkVector4 m_up;
        float m_fov{};
        float m_far{};
        float m_near{};
        bool m_leftHanded{};

    public:
        hkxCamera() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkxCamera::Class;
}
