#pragma once

#include "Havok/Base/hkReferencedObject.h"

namespace nemesis
{
    struct hkaBoneAttachment : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0xa8ccd5cf,
                                                "hkaBoneAttachment",
                                                &nemesis::hkReferencedObject::Class,
                                                128,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkaBoneAttachment);

        nemesis::hkStringPtr m_originalSkeletonName;
        nemesis::hkMatrix4 m_boneFromAttachment;
        nemesis::hkRefPtr<nemesis::hkReferencedObject> m_attachment;
        nemesis::hkStringPtr m_name;
        short m_boneIndex{};

    public:
        hkaBoneAttachment() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkaBoneAttachment::Class;
}
