#pragma once

#include "Havok/Base/hkbModifier.h"

namespace nemesis
{
    struct BSDecomposeVectorModifier : nemesis::hkbModifier
    {
        static constexpr nemesis::hkClass Class{0x31f6b8b6,
                                                "BSDecomposeVectorModifier",
                                                &nemesis::hkbModifier::Class,
                                                112,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(BSDecomposeVectorModifier);

        nemesis::hkVector4 m_vector;
        float m_x{};
        float m_y{};
        float m_z{};
        float m_w{};

    public:
        BSDecomposeVectorModifier() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::BSDecomposeVectorModifier::Class;
}
