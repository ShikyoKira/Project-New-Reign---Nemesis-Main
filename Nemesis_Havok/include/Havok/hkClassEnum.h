#pragma once

#include <array>

#include "Havok/Types/hkEnum.h"
#include "Havok/Types/hkCString.h"

#include "Havok/hkClass.h"
#include "Havok/hkClassEnumItem.h"
#include "Havok/hkCustomAttributes.h"
#include "Havok/hkRefPtr.h"

namespace nemesis
{
    struct hkClassEnum : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0x8a3609cf,
                                                "hkClassEnum",
                                                nullptr,
                                                40,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkClassEnum);

        using Flags = nemesis::hkInternalClassEnumItem<unsigned int>;

        nemesis::hkCString m_name;
        std::array<nemesis::hkClassEnumItem, 0> m_items;
        nemesis::hkRefPtr<nemesis::hkCustomAttributes> m_attributes;
        nemesis::hkEnum<Flags, unsigned int> m_flags = {
            //{"FLAGS_NONE", 0},
            {"0", 0},
        };

    public:
        hkClassEnum() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkClassEnum::Class;
}
