#pragma once

#include "Havok/Base/hkaAnimatedReferenceFrame.h"
#include "Havok/Base/hkpContactMgr.h"

#include "Havok/hkpAgentEntry.h"
#include "Havok/hkpLinkedCollidable.h"

namespace nemesis
{
    struct hkpAgentNnEntry : nemesis::hkpAgentEntry
    {
        static constexpr nemesis::hkClass Class{0x00,
                                                "hkpAgentNnEntry",
                                                &nemesis::hkpAgentEntry::Class,
                                                40,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        using hkpAgentNnTrackType = nemesis::hkInternalClassEnumItem<unsigned char>;

        std::array<unsigned short, 2> m_agentIndexOnCollidable;
        nemesis::hkRefPtr<nemesis::hkpContactMgr> m_contactMgr;
        unsigned char m_collisionQualityIndex;
        unsigned char m_forceCollideOntoPpu;
        nemesis::hkEnum<hkpAgentNnTrackType, unsigned char> m_nnTrackType = {
            {"HK_AGENT3_INVALID_TRACK", 0},
            {"HK_AGENT3_MIDPHASE_TRACK", 1},
            {"HK_AGENT3_NARROWPHASE_TRACK", 2},
        };
        unsigned char m_padding;
        std::array<nemesis::hkRefPtr<nemesis::hkpLinkedCollidable>, 2> m_collidable;

    public:
        hkpAgentNnEntry(const nemesis::hkClass& cls) noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpAgentNnEntry::Class;
}
