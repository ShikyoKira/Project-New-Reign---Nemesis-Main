#pragma once

#include "Havok/hkpSerializedDisplayMarker.h"

namespace nemesis
{
    struct hkpSerializedDisplayMarkerList : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0x54785c77,
                                                "hkpSerializedDisplayMarkerList",
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
        REGISTER_HAVOK_POINTER_HEADER(hkpSerializedDisplayMarkerList);

        nemesis::hkArray<nemesis::hkRefPtr<nemesis::hkpSerializedDisplayMarker>> m_markers;

    public:
        hkpSerializedDisplayMarkerList() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpSerializedDisplayMarkerList::Class;
}
