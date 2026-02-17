#pragma once

#include "Havok/Base/hkReferencedObject.h"

namespace nemesis
{
    struct hkxIndexBuffer : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0xc12c8197,
                                                "hkxIndexBuffer",
                                                &nemesis::hkReferencedObject::Class,
                                                64,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkxIndexBuffer);

        using IndexType = nemesis::hkInternalClassEnumItem<char>;

        nemesis::hkEnum<IndexType, char> m_indexType = {
            {"INDEX_TYPE_INVALID", 0},
            {"INDEX_TYPE_TRI_LIST", 1},
            {"INDEX_TYPE_TRI_STRIP", 2},
            {"INDEX_TYPE_TRI_FAN", 3},
            {"INDEX_TYPE_MAX_ID", 4},
        };
        nemesis::hkArray<unsigned short> m_indices16;
        nemesis::hkArray<unsigned int> m_indices32;
        unsigned int m_vertexBaseOffset{};
        unsigned int m_length{};

    public:
        hkxIndexBuffer() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkxIndexBuffer::Class;
}
