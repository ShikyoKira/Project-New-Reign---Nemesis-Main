#pragma once

#include "Havok/hkClass.h"

#include "Serialize/Deserializer.h"

namespace nemesis
{
    struct hkTrackerSerializableScanSnapshotAllocation : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0x9ab3a6ac,
                                                "hkTrackerSerializableScanSnapshotAllocation",
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
        REGISTER_HAVOK_POINTER_HEADER(hkTrackerSerializableScanSnapshotAllocation);

        nemesis::hkUlong m_start;
        nemesis::hkUlong m_size;
        int m_traceId;

    public:
        hkTrackerSerializableScanSnapshotAllocation() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkTrackerSerializableScanSnapshotAllocation::Class;
}
