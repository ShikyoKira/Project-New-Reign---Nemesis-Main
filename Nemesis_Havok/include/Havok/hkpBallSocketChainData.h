#pragma once

#include "Havok/Base/hkpConstraintChainData.h"

#include "Havok/hkpBridgeAtoms.h"
#include "Havok/hkpBallSocketChainDataConstraintInfo.h"

namespace nemesis
{
    struct hkpBallSocketChainData : nemesis::hkpConstraintChainData
    {
        static constexpr nemesis::hkClass Class{0x102aae9c,
                                                "hkpBallSocketChainData",
                                                &nemesis::hkpConstraintChainData::Class,
                                                80,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpBallSocketChainData);

        nemesis::hkpBridgeAtoms m_atoms;
        nemesis::hkArray<nemesis::hkpBallSocketChainDataConstraintInfo> m_infos;
        float m_tau;
        float m_damping;
        float m_cfm;
        float m_maxErrorDistance;
        bool m_useStabilizedCode;

    public:
        hkpBallSocketChainData() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpBallSocketChainData::Class;
}
