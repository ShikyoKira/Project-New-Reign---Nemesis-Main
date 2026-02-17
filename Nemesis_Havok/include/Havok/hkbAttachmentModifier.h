#pragma once

#include "Havok/Base/hkbModifier.h"

#include "Havok/hkbAttachmentSetup.h"
#include "Havok/hkbEventProperty.h"
#include "Havok/hkbHandle.h"

namespace nemesis
{
    struct hkbAttachmentModifier : nemesis::hkbModifier
    {
        static constexpr nemesis::hkClass Class{0xcc0aab32,
                                                "hkbAttachmentModifier",
                                                &nemesis::hkbModifier::Class,
                                                200,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkbAttachmentModifier);

        nemesis::hkbEventProperty m_sendToAttacherOnAttach;
        nemesis::hkbEventProperty m_sendToAttacheeOnAttach;
        nemesis::hkbEventProperty m_sendToAttacherOnDetach;
        nemesis::hkbEventProperty m_sendToAttacheeOnDetach;
        nemesis::hkRefPtr<nemesis::hkbAttachmentSetup> m_attachmentSetup;
        nemesis::hkRefPtr<nemesis::hkbHandle> m_attacherHandle;
        nemesis::hkRefPtr<nemesis::hkbHandle> m_attacheeHandle;
        int m_attacheeLayer{};
        nemesis::hkRefPtr<nemesis::HavokObject> m_attacheeRB;
        unsigned char m_oldMotionType{};
        int m_oldFilterInfo{};
        nemesis::hkRefPtr<nemesis::HavokObject> m_attachment;

    public:
        hkbAttachmentModifier() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbAttachmentModifier::Class;
}
