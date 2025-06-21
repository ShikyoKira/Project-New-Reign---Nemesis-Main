#pragma once

#include "Havok/hkClass.h"

#include "Serialize/Deserializer.h"

namespace nemesis
{
    struct hkpAgent1nSector : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0x626e55a,
                                                "hkpAgent1nSector",
                                                nullptr,
                                                512,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpAgent1nSector);

        unsigned int m_bytesAllocated;
        unsigned int m_pad0;
        unsigned int m_pad1;
        unsigned int m_pad2;
        std::array<unsigned char, 496> m_data;

    public:
        hkpAgent1nSector() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpAgent1nSector::Class;
}
