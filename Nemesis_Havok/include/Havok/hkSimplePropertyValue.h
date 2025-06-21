#pragma once

#include "Havok/hkClass.h"

#include "Serialize/Deserializer.h"

namespace nemesis
{
    struct hkPropertyValueCommon : nemesis::HavokObject
    {
    private:
        uint64_t m_data;

    public:
        hkPropertyValueCommon() noexcept;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    struct hkpPropertyValue : nemesis::hkPropertyValueCommon
    {
        static constexpr nemesis::hkClass Class{0xc75925aa,
                                                "hkpPropertyValue",
                                                nullptr,
                                                8,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpPropertyValue);

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;
    };

    struct hkSimplePropertyValue : nemesis::hkPropertyValueCommon
    {
        static constexpr nemesis::hkClass Class{0xc75925aa,
                                                "hkSimplePropertyValue",
                                                nullptr,
                                                8,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkSimplePropertyValue);

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;
    };

    constexpr nemesis::hkClass nemesis::hkpPropertyValue::Class;
    constexpr nemesis::hkClass nemesis::hkSimplePropertyValue::Class;
}
