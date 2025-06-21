#pragma once

#include "Havok/hkClass.h"

#include "Serialize/Deserializer.h"

namespace nemesis
{
    struct hkbKeyframeBonesModifierKeyframeInfo : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0x72deb7a6,
                                                "hkbKeyframeBonesModifierKeyframeInfo",
                                                nullptr,
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
        REGISTER_HAVOK_POINTER_HEADER(hkbKeyframeBonesModifierKeyframeInfo);

        nemesis::hkVector4 m_keyframedPosition;
        nemesis::hkQuaternion m_keyframedRotation;
        short m_boneIndex;
        bool m_isValid;

    public:
        hkbKeyframeBonesModifierKeyframeInfo() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbKeyframeBonesModifierKeyframeInfo::Class;
}
