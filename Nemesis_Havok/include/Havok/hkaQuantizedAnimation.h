#pragma once

#include "Havok/Base/hkaAnimation.h"

namespace nemesis
{
    struct hkaQuantizedAnimation : nemesis::hkaAnimation
    {
        static constexpr nemesis::hkClass Class{0x3920f053,
                                                "hkaQuantizedAnimation",
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
        REGISTER_HAVOK_POINTER_HEADER(hkaQuantizedAnimation);

        nemesis::hkArray<unsigned char> m_data;
        unsigned int m_endian{};
        nemesis::hkRefPtr<nemesis::HavokObject> m_skeleton;

    public:
        hkaQuantizedAnimation() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkaQuantizedAnimation::Class;
}
