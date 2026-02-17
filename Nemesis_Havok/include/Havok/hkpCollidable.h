#pragma once

#include "Havok/Base/hkpCdBody.h"

#include "Havok/hkpCollidableBoundingVolumeData.h"
#include "Havok/hkpTypedBroadPhaseHandle.h"

namespace nemesis
{
    struct hkpCollidable : nemesis::hkpCdBody
    {
        static constexpr nemesis::hkClass Class{0x9a0e42a5,
                                                "hkpCollidable",
                                                &nemesis::hkpCdBody::Class,
                                                112,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpCollidable);

        char m_ownerOffset{};
        unsigned char m_forceCollideOntoPpu{};
        unsigned short m_shapeSizeOnSpu{};
        nemesis::hkpTypedBroadPhaseHandle m_broadPhaseHandle;
        nemesis::hkpCollidableBoundingVolumeData m_boundingVolumeData;
        float m_allowedPenetrationDepth{};

    public:
        hkpCollidable() noexcept;
        hkpCollidable(const nemesis::hkClass& cls) noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpCollidable::Class;
}
