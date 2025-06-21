#pragma once

#include "Havok/Base/hkbModifier.h"

#include "Havok/hkbHandle.h"
#include "Havok/hkbSenseHandleModifierRange.h"

namespace nemesis
{
    struct hkbSenseHandleModifier : nemesis::hkbModifier
    {
        static constexpr nemesis::hkClass Class{0x2a064d99,
                                                "hkbSenseHandleModifier",
                                                &nemesis::hkbModifier::Class,
                                                224,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkbSenseHandleModifier);

        using SensingMode = nemesis::hkInternalClassEnumItem<char>;

        nemesis::hkbHandle m_handle;
        nemesis::hkVector4 m_sensorLocalOffset;
        nemesis::hkArray<nemesis::hkbSenseHandleModifierRange> m_ranges;
        nemesis::hkRefPtr<nemesis::hkbHandle> m_handleOut;
        nemesis::hkRefPtr<nemesis::hkbHandle> m_handleIn;
        nemesis::hkStringPtr m_localFrameName;
        nemesis::hkStringPtr m_sensorLocalFrameName;
        float m_minDistance;
        float m_maxDistance;
        float m_distanceOut;
        unsigned int m_collisionFilterInfo;
        short m_sensorRagdollBoneIndex;
        short m_sensorAnimationBoneIndex;
        nemesis::hkEnum<SensingMode, char> m_sensingMode = {
            {"SENSE_IN_NEARBY_RIGID_BODIES", 0},
            {"SENSE_IN_RIGID_BODIES_OUTSIDE_THIS_CHARACTER", 1},
            {"SENSE_IN_OTHER_CHARACTER_RIGID_BODIES", 2},
            {"SENSE_IN_THIS_CHARACTER_RIGID_BODIES", 3},
            {"SENSE_IN_GIVEN_CHARACTER_RIGID_BODIES", 4},
            {"SENSE_IN_GIVEN_RIGID_BODY", 5},
            {"SENSE_IN_OTHER_CHARACTER_SKELETON", 6},
            {"SENSE_IN_THIS_CHARACTER_SKELETON", 7},
            {"SENSE_IN_GIVEN_CHARACTER_SKELETON", 8},
            {"SENSE_IN_GIVEN_LOCAL_FRAME_GROUP", 9},
        };
        bool m_extrapolateSensorPosition;
        bool m_keepFirstSensedHandle;
        bool m_foundHandleOut;
        float m_timeSinceLastModify;
        int m_rangeIndexForEventToSendNextUpdate;

    public:
        hkbSenseHandleModifier() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbSenseHandleModifier::Class;
}
