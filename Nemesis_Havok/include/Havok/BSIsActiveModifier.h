#pragma once

#include "Havok/Base/hkbModifier.h"

namespace nemesis
{
    struct BSIsActiveModifier : nemesis::hkbModifier
    {
        static constexpr nemesis::hkClass Class{0xb0fde45a,
                                                "BSIsActiveModifier",
                                                &nemesis::hkbModifier::Class,
                                                96,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(BSIsActiveModifier);

        bool m_bIsActive0{};
        bool m_bInvertActive0{};
        bool m_bIsActive1{};
        bool m_bInvertActive1{};
        bool m_bIsActive2{};
        bool m_bInvertActive2{};
        bool m_bIsActive3{};
        bool m_bInvertActive3{};
        bool m_bIsActive4{};
        bool m_bInvertActive4{};

    public:
        BSIsActiveModifier() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::BSIsActiveModifier::Class;
}
