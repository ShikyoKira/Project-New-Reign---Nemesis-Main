#pragma once

#include "Havok/Base/hkpShape.h"
#include "Havok/Base/hkpShapeContainer.h"

namespace nemesis
{
    struct hkpShapeCollection : nemesis::hkpShape, nemesis::hkpShapeContainer
    {
        static constexpr nemesis::hkClass Class{0xe8c3991d,
                                                "hkpShapeCollection",
                                                &nemesis::hkpShape::Class,
                                                48,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpShapeCollection);

        using CollectionType = nemesis::hkInternalClassEnumItem<unsigned char>;

        bool m_disableWelding{};
        nemesis::hkEnum<CollectionType, unsigned char> m_collectionType = {
            {"COLLECTION_LIST", 0},
            {"COLLECTION_EXTENDED_MESH", 1},
            {"COLLECTION_TRISAMPLED_HEIGHTFIELD", 2},
            {"COLLECTION_USER", 3},
            {"COLLECTION_SIMPLE_MESH", 4},
            {"COLLECTION_MESH_SHAPE", 5},
            {"COLLECTION_COMPRESSED_MESH", 6},
            {"COLLECTION_MAX", 7},
        };

    public:
        hkpShapeCollection() noexcept;
        hkpShapeCollection(const nemesis::hkClass& cls) noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpShapeCollection::Class;
}
