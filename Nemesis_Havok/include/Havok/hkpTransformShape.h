#pragma once

#include "Havok/Base/hkpShape.h"

#include "Havok/hkpSingleShapeContainer.h"

namespace nemesis
{
    struct hkpTransformShape : nemesis::hkpShape
    {
        static constexpr nemesis::hkClass Class{0x787ef513,
                                                "hkpTransformShape",
                                                &nemesis::hkpShape::Class,
                                                144,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpTransformShape);

        nemesis::hkpSingleShapeContainer m_childShape;
        int m_childShapeSize{};
        nemesis::hkQuaternion m_rotation;
        nemesis::hkTransform m_transform;

    public:
        hkpTransformShape() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpTransformShape::Class;
}
