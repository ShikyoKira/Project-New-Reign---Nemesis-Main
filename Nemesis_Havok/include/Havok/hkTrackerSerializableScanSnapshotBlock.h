#pragma once

#include "Havok/hkClass.h"

#include "Serialize/Deserializer.h"

namespace nemesis
{
    struct hkTrackerSerializableScanSnapshotBlock : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0xe7f23e6d,
                                                "hkTrackerSerializableScanSnapshotBlock",
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
        REGISTER_HAVOK_POINTER_HEADER(hkTrackerSerializableScanSnapshotBlock);

        int m_typeIndex;
        nemesis::hkUlong m_start;
        nemesis::hkUlong m_size;
        int m_arraySize;
        int m_startReferenceIndex;
        int m_numReferences;

    public:
        hkTrackerSerializableScanSnapshotBlock() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkTrackerSerializableScanSnapshotBlock::Class;
}
