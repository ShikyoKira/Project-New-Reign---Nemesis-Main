#pragma once

#include "Havok/Base/hkbModifier.h"

namespace nemesis
{
    struct BSGetTimeStepModifier : nemesis::hkbModifier
    {
        static constexpr nemesis::hkClass Class{0xbda33bfe,
                                                "BSGetTimeStepModifier",
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
        REGISTER_HAVOK_POINTER_HEADER(BSGetTimeStepModifier);

        float m_timeStep{};

    public:
        BSGetTimeStepModifier() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::BSGetTimeStepModifier::Class;
}
