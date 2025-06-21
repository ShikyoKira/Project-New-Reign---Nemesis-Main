#pragma once

#include "Havok/Base/hkaAnimation.h"

namespace nemesis
{
    struct hkaInterleavedUncompressedAnimation : nemesis::hkaAnimation
    {
        static constexpr nemesis::hkClass Class{0x930af031,
                                                "hkaInterleavedUncompressedAnimation",
                                                &nemesis::hkaAnimation::Class,
                                                88,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkaInterleavedUncompressedAnimation);

        nemesis::hkArray<nemesis::hkQsTransform> m_transforms;
        nemesis::hkArray<float> m_floats;

    public:
        hkaInterleavedUncompressedAnimation() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkaInterleavedUncompressedAnimation::Class;
}
