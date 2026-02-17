#pragma once

#include "Havok/hkClass.h"

#include "Serialize/Deserializer.h"

namespace nemesis
{
    struct hkxNodeAnnotationData : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0x433dee92,
                                                "hkxNodeAnnotationData",
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
        REGISTER_HAVOK_POINTER_HEADER(hkxNodeAnnotationData);

        float m_time{};
        nemesis::hkStringPtr m_description;

    public:
        hkxNodeAnnotationData() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkxNodeAnnotationData::Class;
}
