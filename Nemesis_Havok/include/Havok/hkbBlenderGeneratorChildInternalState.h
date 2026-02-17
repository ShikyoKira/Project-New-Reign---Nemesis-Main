#pragma once

#include "Havok/hkClass.h"

#include "Serialize/Deserializer.h"

namespace nemesis
{
    struct hkbBlenderGeneratorChildInternalState : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0xff7327c0,
                                                "hkbBlenderGeneratorChildInternalState",
                                                nullptr,
                                                2,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkbBlenderGeneratorChildInternalState);

        bool m_isActive{};
        bool m_syncNextFrame{};

    public:
        hkbBlenderGeneratorChildInternalState() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbBlenderGeneratorChildInternalState::Class;
}
