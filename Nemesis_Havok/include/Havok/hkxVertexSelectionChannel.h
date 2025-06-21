#pragma once

#include "Havok/Base/hkReferencedObject.h"

namespace nemesis
{
    struct hkxVertexSelectionChannel : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0x866ec6d0,
                                                "hkxVertexSelectionChannel",
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
        REGISTER_HAVOK_POINTER_HEADER(hkxVertexSelectionChannel);

        nemesis::hkArray<int> m_selectedVertices;

    public:
        hkxVertexSelectionChannel() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkxVertexSelectionChannel::Class;
}
