#pragma once

#include "Havok/Base/hkReferencedObject.h"

#include "Havok/hkbEventRangeData.h"

namespace nemesis
{
    struct hkbEventRangeDataArray : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0x330a56ee,
                                                "hkbEventRangeDataArray",
                                                &nemesis::hkReferencedObject::Class,
                                                32,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkbEventRangeDataArray);

        nemesis::hkArray<nemesis::hkbEventRangeData> m_eventData;

    public:
        hkbEventRangeDataArray() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbEventRangeDataArray::Class;
}
