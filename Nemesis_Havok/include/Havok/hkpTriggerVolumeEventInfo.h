#pragma once

#include "Havok/hkpRigidBody.h"

namespace nemesis
{
    struct hkpTriggerVolumeEventInfo : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0xeb60f431,
                                                "hkpTriggerVolumeEventInfo",
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
        REGISTER_HAVOK_POINTER_HEADER(hkpTriggerVolumeEventInfo);

        using Operation = nemesis::hkInternalClassEnumItem<int>;

        uint64_t m_sortValue{};
        nemesis::hkRefPtr<nemesis::hkpRigidBody> m_body;
        nemesis::hkEnum<Operation, int> m_operation = {
            {"ADDED_OP", 0},
            {"REMOVED_OP", 1},
            {"CONTACT_OP", 2},
            {"TOI_OP", 3},
        };

    public:
        hkpTriggerVolumeEventInfo() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpTriggerVolumeEventInfo::Class;
}
