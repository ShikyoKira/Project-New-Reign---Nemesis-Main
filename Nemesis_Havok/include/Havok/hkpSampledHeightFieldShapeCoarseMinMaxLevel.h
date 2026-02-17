#pragma once

#include "Havok/hkClass.h"

#include "Serialize/Deserializer.h"

namespace nemesis
{
    struct hkpSampledHeightFieldShapeCoarseMinMaxLevel : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0x00,
                                                "hkpSampledHeightFieldShapeCoarseMinMaxLevel",
                                                nullptr,
                                                24,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpSampledHeightFieldShapeCoarseMinMaxLevel);

        nemesis::hkArray<nemesis::hkVector4> m_minMaxData;
        int m_xRes{};
        int m_zRes{};

    public:
        hkpSampledHeightFieldShapeCoarseMinMaxLevel() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpSampledHeightFieldShapeCoarseMinMaxLevel::Class;
}
