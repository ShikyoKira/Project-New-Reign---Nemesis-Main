#pragma once

#include "Havok/Base/hkpConstraintChainData.h"

#include "Havok/hkpBridgeAtoms.h"
#include "Havok/hkpStiffSpringChainDataConstraintInfo.h"

namespace nemesis
{
    struct hkpStiffSpringChainData : nemesis::hkpConstraintChainData
    {
        static constexpr nemesis::hkClass Class{0xf170356b,
                                                "hkpStiffSpringChainData",
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
        REGISTER_HAVOK_POINTER_HEADER(hkpStiffSpringChainData);

        nemesis::hkpBridgeAtoms m_atoms;
        nemesis::hkArray<nemesis::hkpStiffSpringChainDataConstraintInfo> m_infos;
        float m_tau{};
        float m_damping{};
        float m_cfm{};

    public:
        hkpStiffSpringChainData() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpStiffSpringChainData::Class;
}
