#pragma once

#include "Havok/Base/hkReferencedObject.h"

#include "Havok/hkpPoweredChainMapperLinkInfo.h"
#include "Havok/hkpPoweredChainMapperTarget.h"
#include "Havok/hkpConstraintChainInstance.h"

namespace nemesis
{
    struct hkpPoweredChainMapper : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0x7a77ef5,
                                                "hkpPoweredChainMapper",
                                                &nemesis::hkReferencedObject::Class,
                                                64,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpPoweredChainMapper);

        nemesis::hkArray<nemesis::hkpPoweredChainMapperLinkInfo> m_links;
        nemesis::hkArray<nemesis::hkpPoweredChainMapperTarget> m_targets;
        nemesis::hkArray<nemesis::hkRefPtr<nemesis::hkpConstraintChainInstance>> m_chains;

    public:
        hkpPoweredChainMapper() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpPoweredChainMapper::Class;
}
