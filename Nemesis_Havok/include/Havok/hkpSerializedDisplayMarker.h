#pragma once

#include "Havok/Base/hkReferencedObject.h"

namespace nemesis
{
    struct hkpSerializedDisplayMarker : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0xd7c8c54f,
                                                "hkpSerializedDisplayMarker",
                                                &nemesis::hkReferencedObject::Class,
                                                80,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpSerializedDisplayMarker);

        nemesis::hkTransform m_transform;

    public:
        hkpSerializedDisplayMarker() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpSerializedDisplayMarker::Class;
}
