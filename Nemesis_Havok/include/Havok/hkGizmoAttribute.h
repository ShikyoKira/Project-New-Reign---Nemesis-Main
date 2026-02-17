#pragma once

#include "Havok/hkClass.h"

#include "Serialize/Deserializer.h"

namespace nemesis
{
    struct hkGizmoAttribute : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0x23aadfb6,
                                                "hkGizmoAttribute",
                                                nullptr,
                                                24,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkGizmoAttribute);

        using GizmoType = nemesis::hkInternalClassEnumItem<char>;

        bool m_visible{};
        nemesis::hkCString m_label;
        nemesis::hkEnum<GizmoType, char> m_type = {
            {"POINT", 0},
            {"SPHERE", 1},
            {"PLANE", 2},
            {"ARROW", 3},
        };

    public:
        hkGizmoAttribute() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkGizmoAttribute::Class;
}
