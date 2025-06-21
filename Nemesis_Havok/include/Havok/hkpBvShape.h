#pragma once

#include "Havok/Base/hkpShape.h"

#include "Havok/hkpSingleShapeContainer.h"

namespace nemesis
{
    struct hkpBvShape : nemesis::hkpShape
    {
        static constexpr nemesis::hkClass Class{0x286eb64c,
                                                "hkpBvShape",
                                                &nemesis::hkpShape::Class,
                                                56,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpBvShape);

        nemesis::hkRefPtr<nemesis::hkpShape> m_boundingVolumeShape;
        nemesis::hkpSingleShapeContainer m_childShape;

    public:
        hkpBvShape() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpBvShape::Class;
}
