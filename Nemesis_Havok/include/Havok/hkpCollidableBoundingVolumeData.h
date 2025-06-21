#pragma once

#include "Havok/hkAabbUint32.h"

namespace nemesis
{
    struct hkpCollidableBoundingVolumeData : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0xb5f0e6b1,
                                                "hkpCollidableBoundingVolumeData",
                                                nullptr,
                                                56,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpCollidableBoundingVolumeData);

        std::array<unsigned int, 3> m_min;
        std::array<unsigned char, 3> m_expansionMin;
        unsigned char m_expansionShift;
        std::array<unsigned int, 3> m_max;
        std::array<unsigned char, 3> m_expansionMax;
        unsigned char m_padding;
        unsigned short m_numChildShapeAabbs;
        unsigned short m_capacityChildShapeAabbs;
        nemesis::hkRefPtr<nemesis::hkAabbUint32> m_childShapeAabbs;
        nemesis::hkRefPtr<nemesis::HavokObject> m_childShapeKeys;

    public:
        hkpCollidableBoundingVolumeData() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpCollidableBoundingVolumeData::Class;
}
