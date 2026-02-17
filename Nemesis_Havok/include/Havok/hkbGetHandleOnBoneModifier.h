#pragma once

#include "Havok/Base/hkbModifier.h"

#include "Havok/hkbHandle.h"

namespace nemesis
{
    struct hkbGetHandleOnBoneModifier : nemesis::hkbModifier
    {
        static constexpr nemesis::hkClass Class{0x50c34a17,
                                                "hkbGetHandleOnBoneModifier",
                                                &nemesis::hkbModifier::Class,
                                                104,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkbGetHandleOnBoneModifier);

        nemesis::hkRefPtr<nemesis::hkbHandle> m_handleOut;
        nemesis::hkStringPtr m_localFrameName;
        short m_ragdollBoneIndex{};
        short m_animationBoneIndex{};

    public:
        hkbGetHandleOnBoneModifier() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbGetHandleOnBoneModifier::Class;
}
