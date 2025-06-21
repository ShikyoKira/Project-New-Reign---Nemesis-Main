#pragma once

#include "Havok/Base/hkbModifier.h"

namespace nemesis
{
    struct hkbModifierWrapper : nemesis::hkbModifier
    {
        static constexpr nemesis::hkClass Class{0x3697e044,
                                                "hkbModifierWrapper",
                                                &nemesis::hkbModifier::Class,
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
        nemesis::hkRefPtr<nemesis::hkbModifier> m_modifier;

    public:
        hkbModifierWrapper(const nemesis::hkClass& cls) noexcept;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbModifierWrapper::Class;
}
