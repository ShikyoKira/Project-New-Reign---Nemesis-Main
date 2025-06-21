#pragma once

#include "Havok/hkClass.h"

#include "Serialize/Deserializer.h"

namespace nemesis
{
    struct hkpConstraintInstanceSmallArraySerializeOverrideType : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0xee3c2aec,
                                                "hkpConstraintInstanceSmallArraySerializeOverrideType",
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
        REGISTER_HAVOK_POINTER_HEADER(hkpConstraintInstanceSmallArraySerializeOverrideType);

        nemesis::hkRefPtr<nemesis::HavokObject> m_data;
        unsigned short m_size;
        unsigned short m_capacityAndFlags;

    public:
        hkpConstraintInstanceSmallArraySerializeOverrideType() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpConstraintInstanceSmallArraySerializeOverrideType::Class;
}
