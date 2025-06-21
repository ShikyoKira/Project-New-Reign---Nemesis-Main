#pragma once

#include "Havok/Base/hkbModifier.h"

#include "Havok/hkbEventProperty.h"

namespace nemesis
{
    struct hkbDetectCloseToGroundModifier : nemesis::hkbModifier
    {
        static constexpr nemesis::hkClass Class{0x981687b2,
                                                "hkbDetectCloseToGroundModifier",
                                                &nemesis::hkbModifier::Class,
                                                120,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkbDetectCloseToGroundModifier);

        nemesis::hkbEventProperty m_closeToGroundEvent;
        float m_closeToGroundHeight;
        float m_raycastDistanceDown;
        unsigned int m_collisionFilterInfo;
        short m_boneIndex;
        short m_animBoneIndex;
        bool m_isCloseToGround;

    public:
        hkbDetectCloseToGroundModifier() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbDetectCloseToGroundModifier::Class;
}
