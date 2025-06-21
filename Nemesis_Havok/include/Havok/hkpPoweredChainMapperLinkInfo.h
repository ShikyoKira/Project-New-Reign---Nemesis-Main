#pragma once

#include "Havok/hkpConstraintInstance.h"

namespace nemesis
{
    struct hkpPoweredChainMapperLinkInfo : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0xcf071a1b,
                                                "hkpPoweredChainMapperLinkInfo",
                                                nullptr,
                                                16,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpPoweredChainMapperLinkInfo);

        int m_firstTargetIdx;
        int m_numTargets;
        nemesis::hkRefPtr<nemesis::hkpConstraintInstance> m_limitConstraint;

    public:
        hkpPoweredChainMapperLinkInfo() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpPoweredChainMapperLinkInfo::Class;
}
