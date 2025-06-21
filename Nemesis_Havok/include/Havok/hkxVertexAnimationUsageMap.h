#pragma once

#include "Havok/hkClass.h"

#include "Serialize/Deserializer.h"

namespace nemesis
{
    struct hkxVertexAnimationUsageMap : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0x46f9168e,
                                                "hkxVertexAnimationUsageMap",
                                                nullptr,
                                                4,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkxVertexAnimationUsageMap);

        using DataUsage = nemesis::hkInternalClassEnumItem<unsigned short>;

        nemesis::hkEnum<DataUsage, unsigned short> m_use = {
            {"HKX_DU_NONE", 0},
            {"HKX_DU_POSITION", 1},
            {"HKX_DU_COLOR", 2},
            {"HKX_DU_NORMAL", 4},
            {"HKX_DU_TANGENT", 8},
            {"HKX_DU_BINORMAL", 16},
            {"HKX_DU_TEXCOORD", 32},
            {"HKX_DU_BLENDWEIGHTS", 64},
            {"HKX_DU_BLENDINDICES", 128},
            {"HKX_DU_USERDATA", 256},
        };
        unsigned char m_useIndexOrig;
        unsigned char m_useIndexLocal;

    public:
        hkxVertexAnimationUsageMap() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkxVertexAnimationUsageMap::Class;
}
