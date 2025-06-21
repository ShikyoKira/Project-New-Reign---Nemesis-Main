#pragma once

#include "Havok/Base/hkReferencedObject.h"

namespace nemesis
{
    struct hkxTriangleSelectionChannel : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0xa02cfca9,
                                                "hkxTriangleSelectionChannel",
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
        REGISTER_HAVOK_POINTER_HEADER(hkxTriangleSelectionChannel);

        nemesis::hkArray<int> m_selectedTriangles;

    public:
        hkxTriangleSelectionChannel() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkxTriangleSelectionChannel::Class;
}
