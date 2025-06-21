#pragma once

#include "Havok/hkpPoweredChainData.h"

namespace nemesis
{
    struct hkpPoweredChainMapperTarget : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0xf651c74d,
                                                "hkpPoweredChainMapperTarget",
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
        REGISTER_HAVOK_POINTER_HEADER(hkpPoweredChainMapperTarget);

        nemesis::hkRefPtr<nemesis::hkpPoweredChainData> m_chain;
        int m_infoIndex;

    public:
        hkpPoweredChainMapperTarget() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpPoweredChainMapperTarget::Class;
}
