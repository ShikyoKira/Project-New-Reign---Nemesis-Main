#pragma once

#include "Havok/hkClass.h"

#include "Serialize/Deserializer.h"

namespace nemesis
{
    struct hkUiAttribute : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0xeb6e96e3,
                                                "hkUiAttribute",
                                                nullptr,
                                                41,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkUiAttribute);

        using HideInModeler = nemesis::hkInternalClassEnumItem<char>;

        bool m_visible{};
        bool m_editable{};
        nemesis::hkEnum<HideInModeler, char> m_hideInModeler = {
            {"NONE", 0},
            {"MAX", 1},
            {"MAYA", 2},
        };
        nemesis::hkCString m_label;
        nemesis::hkCString m_group;
        nemesis::hkCString m_hideBaseClassMembers;
        bool m_endGroup{};
        bool m_endGroup2{};
        bool m_advanced{};

    public:
        hkUiAttribute() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkUiAttribute::Class;
}
