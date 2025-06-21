#pragma once

#include "Havok/Base/hkpConvexTransformShapeBase.h"

namespace nemesis
{
    struct hkpConvexTransformShape : nemesis::hkpConvexTransformShapeBase
    {
        static constexpr nemesis::hkClass Class{0xae3e5017,
                                                "hkpConvexTransformShape",
                                                &nemesis::hkpConvexTransformShapeBase::Class,
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
        REGISTER_HAVOK_POINTER_HEADER(hkpConvexTransformShape);

        nemesis::hkTransform m_transform_0;
        nemesis::hkQsTransform m_transform_1;
        nemesis::hkVector4 m_extraScale;

    public:
        hkpConvexTransformShape() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpConvexTransformShape::Class;
}
