#pragma once

#include "Havok/hkClass.h"

#include "Serialize/Deserializer.h"

namespace nemesis
{
    struct hkpSimpleContactConstraintDataInfo : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0xb59d1734,
                                                "hkpSimpleContactConstraintDataInfo",
                                                nullptr,
                                                32,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpSimpleContactConstraintDataInfo);

        unsigned short m_flags{}; // align 16
        unsigned short m_index{};
        float m_internalData0 = 0;
        unsigned short m_biNormalAxis{};
        nemesis::hkHalf m_rollingFrictionMultiplier;
        nemesis::hkHalf m_internalData1;
        std::array<nemesis::hkHalf, 2> m_rhsRolling;
        float m_contactRadius = 0;
        std::array<unsigned int, 7> m_data_0{};
        std::array<unsigned int, 5> m_data_1{};
        std::array<unsigned int, 4> m_data_2{};

    public:
        hkpSimpleContactConstraintDataInfo() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpSimpleContactConstraintDataInfo::Class;
}
