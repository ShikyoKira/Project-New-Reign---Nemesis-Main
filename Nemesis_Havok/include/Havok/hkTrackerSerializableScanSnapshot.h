#pragma once

#include "Havok/Base/hkReferencedObject.h"

#include "Havok/hkTrackerSerializableScanSnapshotAllocation.h"
#include "Havok/hkTrackerSerializableScanSnapshotBlock.h"

namespace nemesis
{
    struct hkTrackerSerializableScanSnapshot : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0x875af1d9,
                                                "hkTrackerSerializableScanSnapshot",
                                                &nemesis::hkReferencedObject::Class,
                                                128,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkTrackerSerializableScanSnapshot);

        nemesis::hkArray<nemesis::hkTrackerSerializableScanSnapshotAllocation> m_allocations;
        nemesis::hkArray<nemesis::hkTrackerSerializableScanSnapshotBlock> m_blocks;
        nemesis::hkArray<int> m_refs;
        nemesis::hkArray<unsigned char> m_typeNames;
        nemesis::hkArray<unsigned char> m_traceText;
        nemesis::hkArray<uint64_t> m_traceAddrs;
        nemesis::hkArray<int> m_traceParents;

    public:
        hkTrackerSerializableScanSnapshot() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkTrackerSerializableScanSnapshot::Class;
}
