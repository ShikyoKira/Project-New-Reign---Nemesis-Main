#pragma once

#include "Havok/hkbBehaviorGraphData.h"
#include "Havok/hkbBehaviorInfoIdToNamePair.h"

namespace nemesis
{
    struct hkbBehaviorInfo : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0xf7645395,
                                                "hkbBehaviorInfo",
                                                &nemesis::hkReferencedObject::Class,
                                                48,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkbBehaviorInfo);

        uint64_t m_characterId;
        nemesis::hkRefPtr<nemesis::hkbBehaviorGraphData> m_data;
        nemesis::hkArray<nemesis::hkbBehaviorInfoIdToNamePair> m_idToNamePairs;

    public:
        hkbBehaviorInfo() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbBehaviorInfo::Class;
}
