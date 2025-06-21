#pragma once

#include "Havok/Base/hkReferencedObject.h"
#include "Havok/Base/hkpShape.h"

namespace nemesis
{
    struct hkpShapeInfo : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0xea7f1d08,
                                                "hkpShapeInfo",
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
        REGISTER_HAVOK_POINTER_HEADER(hkpShapeInfo);

        nemesis::hkRefPtr<nemesis::hkpShape> m_shape;
        bool m_isHierarchicalCompound;
        bool m_hkdShapesCollected;
        nemesis::hkArray<nemesis::hkCString> m_childShapeNames;
        nemesis::hkArray<nemesis::hkTransform> m_childTransforms;
        nemesis::hkTransform m_transform;

    public:
        hkpShapeInfo() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpShapeInfo::Class;
}
