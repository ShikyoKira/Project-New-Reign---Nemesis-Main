#pragma once

#include "Havok/Base/hkxAttributeHolder.h"

namespace nemesis
{
    struct hkxMeshUserChannelInfo : nemesis::hkxAttributeHolder
    {
        static constexpr nemesis::hkClass Class{0x270724a5,
                                                "hkxMeshUserChannelInfo",
                                                &nemesis::hkxAttributeHolder::Class,
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
        REGISTER_HAVOK_POINTER_HEADER(hkxMeshUserChannelInfo);

        nemesis::hkStringPtr m_name;
        nemesis::hkStringPtr m_className;

    public:
        hkxMeshUserChannelInfo() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkxMeshUserChannelInfo::Class;
}
