#pragma once

#include "Havok/hkSimplePropertyValue.h"

namespace nemesis
{
    struct hkPropertyCommon : nemesis::HavokObject
    {
    private:
        unsigned int m_key;
        unsigned int m_alignmentPadding;

    public:
        hkPropertyCommon() noexcept;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    struct hkpProperty : nemesis::hkPropertyCommon
    {
        static constexpr nemesis::hkClass Class{0x9ce308e9,
                                                "hkpProperty",
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
        REGISTER_HAVOK_POINTER_HEADER(hkpProperty);

        nemesis::hkpPropertyValue m_value;

    public:
        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    struct hkSimpleProperty : nemesis::hkPropertyCommon
    {
        static constexpr nemesis::hkClass Class{0x9ce308e9,
                                                "hkSimpleProperty",
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
        REGISTER_HAVOK_POINTER_HEADER(hkSimpleProperty);

        nemesis::hkSimplePropertyValue m_value;

    public:
        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpProperty::Class;
    constexpr nemesis::hkClass nemesis::hkSimpleProperty::Class;
}
