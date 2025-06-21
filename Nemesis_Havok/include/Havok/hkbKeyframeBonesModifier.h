#pragma once

#include "Havok/Base/hkbModifier.h"

#include "Havok/hkbKeyframeBonesModifierKeyframeInfo.h"
#include "Havok/hkbBoneIndexArray.h"

namespace nemesis
{
    struct hkbKeyframeBonesModifier : nemesis::hkbModifier
    {
        static constexpr nemesis::hkClass Class{0x95f66629,
                                                "hkbKeyframeBonesModifier",
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
        REGISTER_HAVOK_POINTER_HEADER(hkbKeyframeBonesModifier);

        nemesis::hkArray<nemesis::hkbKeyframeBonesModifierKeyframeInfo> m_keyframeInfo;
        nemesis::hkRefPtr<nemesis::hkbBoneIndexArray> m_keyframedBonesList;

    public:
        hkbKeyframeBonesModifier() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbKeyframeBonesModifier::Class;
}
