#pragma once

#include "Havok/hkClass.h"

#include "Serialize/Deserializer.h"

namespace nemesis
{
    struct hkpMoppCodeCodeInfo : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0xd8fdbb08,
                                                "hkpMoppCodeCodeInfo",
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
        REGISTER_HAVOK_POINTER_HEADER(hkpMoppCodeCodeInfo);

        nemesis::hkVector4 m_offset;

    public:
        hkpMoppCodeCodeInfo() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpMoppCodeCodeInfo::Class;
}
