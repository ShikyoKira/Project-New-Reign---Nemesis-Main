#pragma once

#include "Havok/Base/hkbModifier.h"

#include "Havok/hkbFootIkGains.h"
#include "Havok/hkbFootIkModifierLeg.h"
#include "Havok/hkbFootIkModifierInternalLegData.h"

namespace nemesis
{
    struct hkbFootIkModifier : nemesis::hkbModifier
    {
        static constexpr nemesis::hkClass Class{0xed8966c0,
                                                "hkbFootIkModifier",
                                                &nemesis::hkbModifier::Class,
                                                256,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkbFootIkModifier);

        using AlignMode = nemesis::hkInternalClassEnumItem<char>;

        nemesis::hkbFootIkGains m_gains;
        nemesis::hkArray<nemesis::hkbFootIkModifierLeg> m_legs;
        float m_raycastDistanceUp{};
        float m_raycastDistanceDown{};
        float m_originalGroundHeightMS{};
        float m_errorOut{};
        nemesis::hkVector4 m_errorOutTranslation;
        nemesis::hkQuaternion m_alignWithGroundRotation;
        float m_verticalOffset{};
        unsigned int m_collisionFilterInfo{};
        float m_forwardAlignFraction{};
        float m_sidewaysAlignFraction{};
        float m_sidewaysSampleWidth{};
        bool m_useTrackData{};
        bool m_lockFeetWhenPlanted{};
        bool m_useCharacterUpVector{};
        nemesis::hkEnum<AlignMode, char> m_alignMode = {
            {"ALIGN_MODE_FORWARD_RIGHT", 0},
            {"ALIGN_MODE_FORWARD", 1},
        };
        nemesis::hkArray<nemesis::hkbFootIkModifierInternalLegData> m_internalLegData;
        float m_prevIsFootIkEnabled{};
        bool m_isSetUp{};
        bool m_isGroundPositionValid{};
        float m_timeStep{};
        bool m_keepSourceFootEndAboveGround{};

    public:
        hkbFootIkModifier() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbFootIkModifier::Class;
}
