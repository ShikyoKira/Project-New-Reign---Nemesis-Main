#pragma once

#include "Havok/Base/hkpConstraintChainData.h"

#include "Havok/hkpBridgeAtoms.h"
#include "Havok/hkpPoweredChainDataConstraintInfo.h"

namespace nemesis
{
    struct hkpPoweredChainData : nemesis::hkpConstraintChainData
    {
        static constexpr nemesis::hkClass Class{0x38aeafc3,
                                                "hkpPoweredChainData",
                                                &nemesis::hkpConstraintChainData::Class,
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
        REGISTER_HAVOK_POINTER_HEADER(hkpPoweredChainData);

        nemesis::hkpBridgeAtoms m_atoms;
        nemesis::hkArray<nemesis::hkpPoweredChainDataConstraintInfo> m_infos;
        float m_tau{};
        float m_damping{};
        float m_cfmLinAdd{};
        float m_cfmLinMul{};
        float m_cfmAngAdd{};
        float m_cfmAngMul{};
        float m_maxErrorDistance{};

    public:
        hkpPoweredChainData() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpPoweredChainData::Class;
}
