#pragma once

#include "Havok/hkClass.h"

#include "Serialize/Deserializer.h"

namespace nemesis
{
    struct hkpEntitySmallArraySerializeOverrideType : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0xee3c2aec,
                                                "hkpEntitySmallArraySerializeOverrideType",
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
        REGISTER_HAVOK_POINTER_HEADER(hkpEntitySmallArraySerializeOverrideType);

        nemesis::hkRefPtr<nemesis::HavokObject> m_data;
        unsigned short m_size{};
        unsigned short m_capacityAndFlags{};

    public:
        hkpEntitySmallArraySerializeOverrideType() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpEntitySmallArraySerializeOverrideType::Class;
}
