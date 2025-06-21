#pragma once

#include "Havok/Base/hkpUnaryAction.h"

namespace nemesis
{
    struct hkpMouseSpringAction : nemesis::hkpUnaryAction
    {
        static constexpr nemesis::hkClass Class{0x6e087fd6,
                                                "hkpMouseSpringAction",
                                                &nemesis::hkpUnaryAction::Class,
                                                144,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpMouseSpringAction);

        nemesis::hkVector4 m_positionInRbLocal;
        nemesis::hkVector4 m_mousePositionInWorld;
        float m_springDamping;
        float m_springElasticity;
        float m_maxRelativeForce;
        float m_objectDamping;
        unsigned int m_shapeKey;
        nemesis::hkArray<nemesis::hkRefPtr<nemesis::HavokObject>> m_applyCallbacks;

    public:
        hkpMouseSpringAction() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpMouseSpringAction::Class;
}
