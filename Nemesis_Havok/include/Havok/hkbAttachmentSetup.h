#pragma once

#include "Havok/Base/hkReferencedObject.h"

namespace nemesis
{
    struct hkbAttachmentSetup : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0x774632b,
                                                "hkbAttachmentSetup",
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
        REGISTER_HAVOK_POINTER_HEADER(hkbAttachmentSetup);

        using AttachmentType = nemesis::hkInternalClassEnumItem<char>;

        float m_blendInTime{};
        float m_moveAttacherFraction{};
        float m_gain{};
        float m_extrapolationTimeStep{};
        float m_fixUpGain{};
        float m_maxLinearDistance{};
        float m_maxAngularDistance{};
        nemesis::hkEnum<AttachmentType, char> m_attachmentType = {
            {"ATTACHMENT_TYPE_KEYFRAME_RIGID_BODY", 0},
            {"ATTACHMENT_TYPE_BALL_SOCKET_CONSTRAINT", 1},
            {"ATTACHMENT_TYPE_RAGDOLL_CONSTRAINT", 2},
            {"ATTACHMENT_TYPE_SET_WORLD_FROM_MODEL", 3},
            {"ATTACHMENT_TYPE_NONE", 4},
        };

    public:
        hkbAttachmentSetup() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbAttachmentSetup::Class;
}
